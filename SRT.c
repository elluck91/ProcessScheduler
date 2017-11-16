#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "Process.h"
#include "ProcessQueue.h"
#include "Constants.h"
#include "SRT.h"

/*  Precondition: queue is a address to a ProcessQueue. ProcessQueue is filled with 
    Processes to be processed.This algorithm stimulates shortest remaining time, 
    preemptive style scheduling.

    Postcondtion: returns a address to a ProcessQueue that contains all Processes
    that were finished. It is ordered by their finish time, from first to last.
*/
struct ProcessQueue * shortest_remaining_time_first(struct ProcessQueue * queue){
    sort_by_arrival_time(queue);
    struct ProcessQueue * finishedProcesses = make_queue( size(queue) );
    struct ProcessQueue * hasArrivedProcesses = make_queue( size(queue)  );
    if( is_empty(queue) ) 
       return NULL; 

    struct Process * next=  peek(queue);
    int nextArrival = next->arrival_time;
    int time = 0 ;
    while(time<=MAX_QUANTA){
        while(time==nextArrival && !is_empty(queue )){
            struct Process * newProcess= dequeue(queue);
            enqueue(hasArrivedProcesses,newProcess);
            if(!is_empty(queue))
                nextArrival = peek(queue)->arrival_time;
                sort_by_remaining(hasArrivedProcesses);
        }
        if(!is_empty(hasArrivedProcesses)){
            struct Process * shortest = peek(hasArrivedProcesses);
            if(shortest->cumulative_time ==0 )
                shortest->response_time = time - shortest->arrival_time;
            shortest->cumulative_time+=1;
            printf("%d,", shortest->id );
            if(shortest->cumulative_time == shortest->service_time){
                struct Process * finisheditem = dequeue(hasArrivedProcesses);
                finisheditem->turnaround_time= time - finisheditem->arrival_time + 1;
                finisheditem->wait_time = finisheditem->turnaround_time - finisheditem->service_time;
                enqueue(finishedProcesses , finisheditem);
            }
        }
        time++;
    }
    int amount = size(hasArrivedProcesses);

    while(amount>0){
        struct Process * process= dequeue(hasArrivedProcesses);
        if(process->cumulative_time!=0){
           process->turnaround_time = process->service_time - process->cumulative_time + 100- process->arrival_time;
           process->wait_time = process->turnaround_time - process->service_time;
           enqueue(finishedProcesses, process);
        }
        amount--;
    }
    printf("\n\n");
    return finishedProcesses;
}




