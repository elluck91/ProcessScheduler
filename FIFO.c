#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "Process.h"
#include "ProcessQueue.h"
#include "Constants.h"
#include "FIFO.h"

/*  Precondition: queue is a address to a ProcessQueue. ProcessQueue is filled with 
    Processes to be processed.This algorithm stimulates first come first serve, 
    non-preemptive style scheduling.

    Postcondtion: returns a address to a ProcessQueue that contains all Processes
    that were finished. It is ordered by their finish time, from first to last.
*/
struct ProcessQueue * first_in_first_out( struct ProcessQueue * queue){
    sort_by_arrival_time(queue);
    struct ProcessQueue * finishedProcesses= make_queue( size(queue) );
    struct ProcessQueue * hasArrivedProcesses = make_queue( size(queue) );
    if( is_empty(queue))
        return NULL;
    int nextArrival = peek(queue)->arrival_time;
    int time = 0 ;
    while( time <=MAX_QUANTA){
        while(time==nextArrival && !is_empty(queue )){
            struct Process * finisheditem = dequeue(queue);
            enqueue(hasArrivedProcesses,finisheditem);
            if(!is_empty(queue))
                nextArrival = peek(queue)->arrival_time;
        }
        if(!is_empty(hasArrivedProcesses)){
            struct Process * first = peek(hasArrivedProcesses);
            if(first->cumulative_time ==0 )
                first->response_time = time - first->arrival_time;
            first->cumulative_time+=1;
            printf("%d,", first->id );
            if(first->cumulative_time == first->service_time){
                struct Process * finisheditem = dequeue(hasArrivedProcesses);
                finisheditem->turnaround_time= time - finisheditem->arrival_time + 1;
                finisheditem->wait_time = finisheditem->turnaround_time - finisheditem->service_time;
                enqueue(finishedProcesses , finisheditem);
            }
        }
        time++;
    }
    if(!is_empty(hasArrivedProcesses)){
       struct Process * process= dequeue(hasArrivedProcesses);
       if(process->cumulative_time!=0){
            process->turnaround_time = process->service_time - process->cumulative_time + 100- process->arrival_time;
            process->wait_time = process->turnaround_time - process->service_time;
            enqueue(finishedProcesses, process);
        }
    }
    printf("\n\n");
    return finishedProcesses;
}





