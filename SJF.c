#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "Process.h"
#include "ProcessQueue.h"
#include "SJF.h"
#include "Constants.h"

/*
 * shortest_job_first simulates shortest job first process scheduling.
 * Precondition: ProcessQueue argument is populated with Processes.
 * Postcondition: If the argument is empty, returns NULL. Otherwise, returns
 * a ProcessQueue containing Processes that have been fully serviced.
 */
struct ProcessQueue * shortest_job_first(struct ProcessQueue *incoming_processes){
   // If the ProcessQueue argument is not empty, simulate the shortest job first algorithm.
   if(!is_empty(incoming_processes)){
      sort_by_arrival_time(incoming_processes);
      struct ProcessQueue *ready_queue= make_queue(MAX_SIZE);
      struct ProcessQueue *result = make_queue(MAX_SIZE);

      struct Process *process;
      int quanta = 0;
      // while there are incoming processes or processes to service,
      // and there is still time to respond to processes...
      while((is_empty(incoming_processes) != true || is_empty(ready_queue) != true) && quanta < MAX_QUANTA){
         // If the ready_queue is empty, enqueue with all processes that have arrived.
         if(is_empty(ready_queue) == true){
            printf("IDLE, ");
            quanta++;
            bool can_queue = !is_empty(incoming_processes);
            while(can_queue){
               if(quanta >= peek(incoming_processes)->arrival_time){
                  enqueue(ready_queue, dequeue(incoming_processes));
                  can_queue = !is_empty(incoming_processes);
               }
               else
                  can_queue = false;
            }
         }
         // Otherwise, respond to the next process in the ready_queue.
         else{
            process = dequeue(ready_queue);
            process->response_time = quanta - process->arrival_time;
            process->wait_time = process->response_time;
            process->turnaround_time = process->service_time;
            process->cumulative_time = process->service_time;
            quanta += process->service_time;
            // Print the id of the process for every quanta it has been processed
            int i;
            for(i = 0; i < process->service_time; i++){
               printf("%d, ", process->id);
            }

            // Enqueue any processes that have arrived.
            bool can_queue = !is_empty(incoming_processes);
            while(is_empty(incoming_processes) == false && can_queue){
               if(quanta >= peek(incoming_processes)->arrival_time){
                  enqueue(ready_queue, dequeue(incoming_processes));
                  can_queue = is_empty(incoming_processes);
               }
               else
                  can_queue = false;
            }
            // Put the serviced process into the queue of results.
            enqueue(result, process);
         }
      }
      puts("\n\nPrinting Result.");
      print_queue(result);
      destroy_queue(ready_queue);
      return result;
   }
   else{
      puts("Process Queue was empty.");
      return NULL;
   }
}
