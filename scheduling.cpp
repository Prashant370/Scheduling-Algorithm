/* 
    Created By Prashant Kumar
    2022CSB1202
*/

#include<bits/stdc++.h>
using namespace std;
// packets of process
#define MAX 1000
const double dx = 0.1;
const double epsilon = 1e-6;
struct Process{
    // input
    char process_name[10];
    double arrival_time;
    double execution_time;
    double cpu_burst_time;
    double io_time;
    int priority;
    // index
    int id;
    // output
    double turn_around_time;
    double total_wait_time; // turn_around_time - burst_time
    double avg_turn_around_time;
    double completion_time;
    double rem_process_time; // for RR only
    bool done; // for sjf only 
    double rem_cpu_burst_time; // for sjf only 
};

struct items{
    int id;
    double arrival_time;
    double rem_et;
    int priority;
    items(int id,double arrival_time,double rem_et,int priority){
        this->id = id;
        this->arrival_time = arrival_time;
        this->rem_et = rem_et;
        this->priority = priority;
    }
};
struct cmp{
    bool operator()(items &a,items &b) {
        if (a.arrival_time != b.arrival_time)
            return a.arrival_time > b.arrival_time;  
        return a.priority > b.priority;
    }
};
FILE * fout ;
void FCFS(int size,vector<Process> processes){// first come first served
    priority_queue<items,vector<items>,cmp> pq;
    for(int i = 0; i < size; i++){
        pq.push(items(processes[i].id,processes[i].arrival_time,processes[i].execution_time,processes[i].priority));
    }
    double curr_time = 0;
    while(!pq.empty()){
        items temp = pq.top();
        pq.pop();
        int id = temp.id;
        double at = temp.arrival_time;
        double et = temp.rem_et;
        int priority = temp.priority;
        double cpu_bt = processes[id].cpu_burst_time;
        double io_time = processes[id].io_time;
        processes[id].total_wait_time += max(0.0,curr_time - at);// calculate waiting time in the ready queue
        curr_time = max(curr_time,at);
        if(et-cpu_bt <= 0){
            curr_time += et;
            processes[id].completion_time = curr_time;
        }
        else{
            curr_time += cpu_bt;
            pq.push(items(id,curr_time+io_time,et-cpu_bt,priority));
        }
    }
    
    fprintf(fout,"(1) FCFS [First Come First Served]\n");
    fprintf(fout,"-------------------------------\n");
    fprintf(fout,"Process Name | Turn Around Time | Wait Time\n");
    double sum = 0;
    for(int i = 0;i < size; i++){
        double ct = processes[i].completion_time;
        double at = processes[i].arrival_time;
        double et = processes[i].execution_time;
        double wt = processes[i].total_wait_time;
        sum += ct-at;
        fprintf(fout,"%s           | %lf        | %lf\n",processes[i].process_name,ct-at,wt);
    }
    fprintf(fout,"Average Turn Around Time -> %lf \n\n\n",sum/size);
}

struct packet_sjf{
    int id;
    int priority;
    double rem_et;
    double rem_cpu_burst_time;
    double arrival_time;
    packet_sjf(int id,double rem_cpu_burst_time,double arrival_time,double rem_et,int priority){
        this->id = id;
        this->rem_cpu_burst_time = rem_cpu_burst_time;
        this->arrival_time = arrival_time;
        this->priority = priority;
        this->rem_et = rem_et;
    }
};

struct comp{
    bool operator()(packet_sjf &a,packet_sjf &b){
        if(a.rem_cpu_burst_time != b.rem_cpu_burst_time)
            return a.rem_cpu_burst_time > b.rem_cpu_burst_time;
        return a.priority > b.priority; 
    }
};

bool check(vector<Process> &processes,double curr_time){
    for(int i = 0; i < processes.size();i++){
        if(!processes[i].done){
            return false;
        }
    }
    return true;
}
void SJF(int size,vector<Process> processes){// shortest job first

    priority_queue<packet_sjf,vector<packet_sjf>,comp> pq;
    vector<double> updated_at(size);
    for(int i = 0; i < size; i++){
        processes[i].rem_process_time = processes[i].cpu_burst_time;
        processes[i].rem_cpu_burst_time = processes[i].cpu_burst_time;
        updated_at[i] = processes[i].arrival_time; 
    }
    double curr_time = 0;
    while(1){
        int cnt = 0;
        for(int i = 0; i < size; i++){
            if(processes[i].done){
                cnt++;
            }
        }
        if(cnt == size) break;
        for(int i = 0; i < size; i++){
            if(!processes[i].done && abs(curr_time - updated_at[i]) <= epsilon){
                pq.push(packet_sjf(processes[i].id,processes[i].rem_cpu_burst_time,curr_time,processes[i].execution_time,processes[i].priority));
            }
        }
        if(pq.empty()) {
            curr_time += dx;
            continue;
        }
        auto temp = pq.top();
        pq.pop();
        int id = temp.id;
        double rem_cpu_burst_time = temp.rem_cpu_burst_time;
        double at = temp.arrival_time;
        double et = temp.rem_et;
        processes[id].total_wait_time += curr_time - at;
        curr_time = max(curr_time,at);
        if(processes[id].rem_cpu_burst_time - dx <= epsilon){
            if(et - dx <= epsilon){
                curr_time += dx;
                processes[id].completion_time = curr_time;
                processes[id].done = true;
            }
            else curr_time += dx;
            processes[id].rem_cpu_burst_time = processes[id].cpu_burst_time;
            processes[id].execution_time -= dx;
            updated_at[id] = curr_time+processes[id].io_time;
            
        }
        else{
            if(et - dx <= epsilon){
                curr_time += dx;
                processes[id].completion_time = curr_time;
                processes[id].done = true;
            }
            else curr_time += dx;
            processes[id].rem_cpu_burst_time -= dx;
            processes[id].execution_time -= dx;
            updated_at[id] = curr_time;
        }       
    }

    fprintf(fout,"(2) SJF [Shortest Job First]\n");
    fprintf(fout,"-------------------------------\n");
    fprintf(fout,"Process Name | Turn Around Time | Wait Time\n");
    double sum = 0;
    for(int i = 0;i < size; i++){
        double ct = processes[i].completion_time;
        double at = processes[i].arrival_time;
        double wt = processes[i].total_wait_time;
        sum += ct-at;
        fprintf(fout,"%s           | %lf        | %lf\n",processes[i].process_name,ct-at,wt);
    }
    fprintf(fout,"Average Turn Around Time -> %lf \n\n\n",sum/size);
}
pair<double,double> RR(int size,vector<Process> processes,double tq,bool flag){// Round Robin Scheduling
    priority_queue<items,vector<items>,cmp> pq;
    // tq can take it from user but for now we can hard code it for 1.5 ms
    for(int i = 0; i < size; i++){
        processes[i].rem_process_time = processes[i].cpu_burst_time;
        pq.push(items(processes[i].id,processes[i].arrival_time,processes[i].execution_time,processes[i].priority));
    }
    double curr_time = 0;
    while(!pq.empty()){
        items temp = pq.top();
        pq.pop();
        int id = temp.id;
        double at = temp.arrival_time;
        double et = temp.rem_et;
        int priority = temp.priority;
        double cpu_bt = processes[id].cpu_burst_time;
        double io_time = processes[id].io_time;
        processes[id].total_wait_time += max(0.0,curr_time - at);// calculate waiting time in the ready queue
        curr_time = max(curr_time,at);
        if(tq >= processes[id].rem_process_time){
            if(processes[id].rem_process_time == cpu_bt){
                if(et-cpu_bt <= 0){
                    curr_time += et;
                    processes[id].completion_time = curr_time;
                }
                else{
                    curr_time += cpu_bt;
                    pq.push(items(id,curr_time+io_time,et-cpu_bt,priority));
                }
            }
            else{
                // less than 
                if(et-processes[id].rem_process_time <= 0){
                    curr_time += et;
                    processes[id].completion_time = curr_time;
                }
                else{
                    curr_time += processes[id].rem_process_time;
                    pq.push(items(id,curr_time+io_time,et-processes[id].rem_process_time,priority));
                    processes[id].rem_process_time = cpu_bt;
                }    
            }
        }
        else{
            if(et-tq <= 0){
                curr_time += et;
                processes[id].completion_time = curr_time;
            }
            else{
                curr_time += tq;
                processes[id].rem_process_time -= tq;
                pq.push(items(id,curr_time,et-tq,priority));
            }            
        }
    }
    if(flag){
        fprintf(fout,"(2) RR [Round Robin] For Tq = %.2lf ms\n",tq );
        fprintf(fout,"-------------------------------\n");
        fprintf(fout,"Process Name | Turn Around Time | Wait Time\n");
    }
    double sum_tat = 0;
    double sum_wt = 0;
    for(int i = 0;i < size; i++){
        double ct = processes[i].completion_time;
        double at = processes[i].arrival_time;
        double et = processes[i].execution_time;
        double wt = processes[i].total_wait_time;
        sum_tat += ct-at;
        sum_wt += wt;
        if(flag)
        fprintf(fout,"%s           | %lf        | %lf\n",processes[i].process_name,ct-at,wt);
    }
    if(flag)
    fprintf(fout,"Average Turn Around Time -> %lf \n",sum_tat/size);
    return {sum_tat/size,sum_wt/size};
}

void have_data_for_graph(int size,vector<Process> processes){
    fprintf(fout,"**************\n");
    fprintf(fout,"Data For Graph\n");
    fprintf(fout,"**************\n");
    fprintf(fout,"Quantum Time | Average Turn Around Time | Average Waiting Time\n");
    for(int i = 1; i <= 10; i++){
        auto it = RR(size,processes,i,0);
        fprintf(fout,"%d            |%lf                 | %lf\n",i,it.first,it.second);
    }
}
int main(){
    FILE *fin;
    fin = fopen("input.txt", "r");
    if (fin == NULL) {
        printf("Error !!\n");
        return 1;
    }
    int i = 0;
    vector<Process> processes(MAX);
    while(fscanf(fin, "%s %lf %lf %lf %lf %d", processes[i].process_name, &processes[i].arrival_time, &processes[i].execution_time, &processes[i].cpu_burst_time, &processes[i].io_time,&processes[i].priority) != EOF){
        processes[i].id = i;
        processes[i].done = false;
        processes[i].completion_time = processes[i].arrival_time;
        i++;
    }
    fout = fopen("output.txt","w");
    FCFS(i,processes);
    SJF(i,processes);
    RR(i,processes,3,1);
    have_data_for_graph(i,processes);
    fclose(fin);
    
    return 0;
}
