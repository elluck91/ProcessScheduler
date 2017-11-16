#ifndef PROCESS_H
#define PROCESS_H
struct Process{
   int id;
   int arrival_time;
   int service_time;
   int remaining_time;
   int priority;
   int wait_time;
   int response_time;
   int turnaround_time;
   int cumulative_time;

};

void print_process(struct Process process);
void print_processes(struct Process processes[], int size);
#endif
