# Process Scheduling Algorithms

This project implements three popular process scheduling algorithms: FCFS (First Come First Serve), SJF (Shortest Job First with preemption), and RR (Round Robin). The algorithms are implemented in C++ and provide various performance metrics such as average turnaround time and average waiting time. The implementation also generates data for visualizing the impact of quantum time on the performance of the Round Robin algorithm.

## Scheduling Algorithms

1. **FCFS (First Come First Serve)**
   - This algorithm schedules processes based on their arrival time, ensuring that the first process to arrive is the first to be executed.
   - Simple to implement but may lead to the "convoy effect" where shorter processes wait for longer ones.

2. **SJF (Shortest Job First) [with preemption]**
   - In this approach, processes with shorter CPU burst times are prioritized for execution. Preemption allows a shorter job to interrupt a running process if it arrives mid-execution.
   - SJF minimizes the average waiting time but requires knowledge of the CPU burst time in advance.

3. **RR (Round Robin)**
   - Each process is assigned a time quantum and execution is cycled between all processes. If a process isn't completed within its quantum, it is returned to the queue.
   - Round Robin is fair and prevents starvation, but the choice of time quantum has a large impact on performance.

## Project Structure

- **input.txt**: Contains process data, including process name, arrival time, execution time, CPU burst time, I/O time, and priority.
- **output.txt**: Outputs the results for each scheduling algorithm, including Turnaround Time, Waiting Time, and Average Turnaround Time.
- **Graphs**: Data generated for graphing Quantum Time vs Average Turnaround Time and Quantum Time vs Average Waiting Time.
## How to Run

1. Compile the program:
   ```bash
   g++ -o scheduling scheduling.cpp

   
## Input Looks Like
![image](https://github.com/user-attachments/assets/ed0ad744-d490-443e-b8c9-ddd7dbadad5b)

## Output Looks Like 
![image](https://github.com/user-attachments/assets/81051b04-2873-4338-8488-e63083aeca0f)
