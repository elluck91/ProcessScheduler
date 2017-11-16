#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <ctype.h>
#include <time.h>
#include "Constants.h"
#include "Process.h"
#include "ProcessQueue.h"
#include "SRT.h"
#include "SJF.h"
#include "FIFO.h"
#include "RoundRobin.h"
#include "HPFP.h"
#include "HPFNP.h"

int main(){
   int seed = time(NULL);
   struct Process processes[MAX_SIZE];
   struct ProcessQueue *queue = generate_process_queue(MAX_SIZE, seed);
   struct ProcessQueue *FCFS_queue, *SJF_queue, *SRT_queue, *RR_queue, *HPFNP_queue, *HPFP_queue;
   FCFS_queue = SJF_queue = SRT_queue = RR_queue = HPFNP_queue = HPFP_queue = NULL;

   int countFCFS = 0;
   int countSJF = 0;
   int countSRT = 0;
   int countRR = 0;
   int countHPFNP = 0;
   int countHPFP = 0;

   long long_input;
   char str_input[2];
   do{
      puts("\n   Process Scheduler");
      puts("0. Exit");
      puts("1. Generate Same Set of Processes");
      puts("2. Generate New Set of Processes");
      puts("3. Print Processes");
      puts("4. First Come First Serve");
      puts("5. Shortest Job First");
      puts("6. Shortest Remaining Time");
      puts("7. Round Robin");
      puts("8. Highest Priority First (non-preemptive)");
      puts("9. Highest Priority First (preemptive)");
      printf("> ");
      fgets(str_input, 2, stdin);
      puts("");
      while(getchar() != '\n'){}
      if(isdigit(str_input[0])){
         long_input = strtol(str_input, NULL, 10);
         switch(long_input){
            case 0:
               puts("Exiting.");
               break;
            case 1:
               puts("Generating Same Set of Processes");
               destroy_queue(queue);
               queue = generate_process_queue(MAX_SIZE, seed);
               break;
            case 2:
               puts("Generating New Set of Processes.");
               destroy_queue(queue);
               seed = time(NULL);
               queue = generate_process_queue(MAX_SIZE, seed);
               break;
            case 3:
               puts("Printing processes.\n");
               print_queue_fresh(queue);
               break;
            case 4:
               puts("Simulating First Come First Serve.\n");
               if(FCFS_queue != NULL){
                  destroy_queue(FCFS_queue);
               }
               FCFS_queue= first_in_first_out(queue);
               print_queue(FCFS_queue);
               destroy_queue(queue);
               queue = generate_process_queue(MAX_SIZE, seed);
	       countFCFS++;
	       printf("\n\nNumber of times run: %d\n\n", countFCFS);
               break;
            case 5:
               puts("Simulating Shortest Job First.\n");
               if(SJF_queue != NULL){
                  destroy_queue(SJF_queue);
               }
               SJF_queue = shortest_job_first(queue);
               destroy_queue(queue);
               queue = generate_process_queue(MAX_SIZE, seed);
	       countSJF++;
	       printf("\n\nNumber of times run: %d\n\n", countSJF);
               break;
            case 6:
               puts("Simulating Shortest Time Remaining.\n");
               if(SRT_queue !=NULL){
                destroy_queue(SRT_queue);    
               }
               SRT_queue = shortest_remaining_time_first(queue);
               destroy_queue(queue);
               print_queue(SRT_queue);
               queue = generate_process_queue(MAX_SIZE,seed);
	       countSRT++;
	       printf("\n\nNumber of times run: %d\n\n", countSRT);
               break;
            case 7:
               puts("Simulating Round Robin.\n");
               if(RR_queue != NULL) {
                  destroy_queue(RR_queue);
               }
               queue = generate_process_queue(MAX_SIZE, seed);
               sort_by_arrival_time(queue);
               RoundRobinAlgorithm(queue);
	       countRR++;
	       printf("\n\nNumber of times run: %d\n\n", countRR);
               break;
            case 8:
               puts("Simulating Highest Priority First (non-preemptive).\n");
               if(HPFNP_queue != NULL) {
                  destroy_queue(HPFNP_queue);
               }
               queue = generate_process_queue(MAX_SIZE, seed);
               sort_by_arrival_time(queue);
               processHPFNP(queue);
	       countHPFNP++;
	       printf("\n\nNumber of times run: %d\n\n", countHPFNP);
               break;
            case 9:
               puts("Simulating Highest Priority First (preemptive).\n");
               if(HPFP_queue != NULL) {
                  destroy_queue(HPFP_queue);
               }
               queue = generate_process_queue(MAX_SIZE, seed);
               sort_by_arrival_time(queue);
               processHPFP(queue);
	       countHPFP++;
	       printf("\n\nNumber of times run: %d\n\n", countHPFP);
               break;
            default:
               puts("Input an integer between 0 and 7 inclusive.");
               break;
         }
      }
      else
         puts("Input an integer between 0 and 7 inclusive.");
   } while(long_input != 0);
   destroy_queue(queue);
   // TODO: Destroy any other queues used.
}
