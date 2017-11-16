#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "Constants.h"
#include "Process.h"
#include "ProcessQueue.h"

const struct Process NULL_PROCESS = {.id = -1, .arrival_time = -1, .service_time = -1, .priority = -1};

/*
 * Constructor for ProcessQueues. Must allocate memory because
 * the ProcessQueue must be on the heap to return it.
 */
struct ProcessQueue * make_queue(int size){
   struct ProcessQueue *queue = malloc(sizeof(struct ProcessQueue));
   queue->process_queue = (struct Process *) malloc (size*sizeof(struct Process));
   queue->max_size = size;
   queue->size = 0;
   queue->last = -1;
   return queue;
}

/*
 * Destructor for ProcessQueues. Must manually free memory
 * because the ProcessQueue struct allocates memory on the heap.
 */
void destroy_queue(struct ProcessQueue *queue){
   free(queue->process_queue);
   free(queue);
}

/*
 * Return the size of a ProcessQueue struct.
 */   
int size(struct ProcessQueue *queue){
   return queue->size;
}

/*
 * Returns the next element in the ProcessQueue. Note, the caller must check
 * the return value for NULL (use '#include <stddef.h>' to define NULL.
 */
struct Process * peek(struct ProcessQueue *queue){
   if(queue != NULL && queue->size !=0){
         return &queue->process_queue[0];
   }else
      return NULL;
    


}
/*
 * Returns true if the ProcessQueue is full, false otherwise.
 */
bool is_full(struct ProcessQueue *queue){
   return queue->max_size == queue->size;
}

/*
 * Returns true if the ProcessQueue is empty, false otherwise.
 */
bool is_empty(struct ProcessQueue *queue){
   return (queue->size == 0);
}

/*
 * Dequeues an element from the ProcessQueue and returns that element. Note, caller must
 * check return value for NULL_PROCESS (check if return.id == -1).
 */
struct Process * dequeue(struct ProcessQueue *queue){
   if(is_empty(queue) == false){
      queue->size -= 1;
      queue->last -= 1;
      return _normalize(queue);
   }
   else
      return NULL;
}

/*
 * Enqueue the passed Process to the passed ProcessQueue.
 */
bool enqueue(struct ProcessQueue *queue, struct Process *process){
   if(queue->max_size != queue->size){
      queue->size += 1;
      queue->last += 1;
      queue->process_queue[queue->last] = *process;
      return true;
   }
   else
      return false;
}

/*
 * Helper function used after dequeueing
 */
struct Process * _normalize(struct ProcessQueue *queue){
   int i;
   struct Process *first = malloc(sizeof(struct Process));
   *first = queue->process_queue[0];
   if(queue->size == 0){
      queue->process_queue[0] = queue->process_queue[1];
   }
   else{
      for(i = 0; i < queue->last + 1; i++){
         queue->process_queue[i] = queue->process_queue[i+1];
      }
   }
   struct Process *processes = queue->process_queue;
   for(i = queue->last + 1; i < queue->max_size; i++){
      processes[i] = NULL_PROCESS;
   }
   return first;
}

/*
 * Sorts the given ProcessQueue by Process.arrival_time.
 */
void sort_by_arrival_time(struct ProcessQueue *queue){
   struct Process *processes = queue->process_queue;
   int index_of_earliest;
   struct Process swap;
   int i, j;
   for(i = 0; i < queue->size; i++) {
      index_of_earliest = i;
      for(j = i; j < queue->size; j++) {
         if(processes[j].arrival_time < processes[index_of_earliest].arrival_time) {
            index_of_earliest = j;
         }
      }
      swap = processes[i];
      processes[i] = processes[index_of_earliest];
      processes[index_of_earliest] = swap;
   }
}

/* 
 * Sorts the given ProcessQueue by Process.service_time.
 */
void sort_by_service_time(struct ProcessQueue *queue){
   struct Process *processes = queue->process_queue;
   int index_of_earliest;
   struct Process swap;
   int i, j;
   for(i = 0; i < queue->size; i++) {
      index_of_earliest = i;
      for(j = i; j < queue->size; j++) {
         if(processes[j].service_time < processes[index_of_earliest].service_time) {
            index_of_earliest = j;
         }
      }
      swap = processes[i];
      processes[i] = processes[index_of_earliest];
      processes[index_of_earliest] = swap;
   }
}

/*
 * Sorts the given ProcessQueue by Process.priority. If two
 * processes are adjacent and have the same priority, they are not
 * swapped.
 */
void sort_by_priority(struct ProcessQueue *queue){
   struct Process *processes = queue->process_queue;
   int index_of_earliest;
   struct Process swap;
   int i, j;
   for(i = 0; i < queue->size; i++) {
      index_of_earliest = i;
      for(j = i; j < queue->size; j++) {
         if(processes[j].priority < processes[index_of_earliest].priority) {
            index_of_earliest = j;
         }
      }
      swap = processes[i];
      processes[i] = processes[index_of_earliest];
      processes[index_of_earliest] = swap;
   }
}

/*
 * Returns a ProcessQueue allocated on the heap and populated with randomly generated
 * Processes. The generated processes are dependent on a passed seed in order to allow
 * for the same set of processes to be generated.
 */
struct ProcessQueue * generate_process_queue(int size, int seed){
   struct ProcessQueue * process_queue = make_queue(size);

   srand(seed);
   int i;
   for(i = 0; i < size; i++) {
      struct Process *process = malloc(size*sizeof(struct Process));
      process->id = i;
      process->arrival_time = rand() % 100;
      process->service_time = rand() % 11;
      if(process->service_time == 0) {
         process->service_time += 1;
      }
      process->priority = rand() % 5;
      if(process->priority == 0) {
         process->priority += 1;
      }
      process->cumulative_time = 0;
      process->wait_time = -1;
      process->response_time = -1;
      process->turnaround_time = -1;
      process->remaining_time = process->service_time;
      enqueue(process_queue, process);
   }
   return process_queue;
}

void _print_process(struct Process process) {
   printf("%-2d%14d%14d%10d%11d%15d%17d%17d\n", process.id, process.arrival_time, process.service_time, process.priority, process.wait_time, process.response_time, process.turnaround_time, process.cumulative_time);
}

void print_queue(struct ProcessQueue * queue) {
	float total_turnaround_time = 0;
	float total_wait_time = 0;
	float total_response_time = 0;
	int processed = 0;

   if(queue != NULL){
      printf("%-2s%14s%14s%10s%11s%15s%17s%17s\n\n", "ID", "Arrival Time", "Service Time", "Priority", "Wait Time", "Response Time", "Turnaround Time", "Cumulative Time");
      int i;
      for(i = 0; i < queue->size; i++) {
         //_print_process(queue->process_queue[i]);
	if(queue->process_queue[i].wait_time != -1 && queue->process_queue[i].response_time <= 99) {
		_print_process(queue->process_queue[i]);
		total_turnaround_time = total_turnaround_time + queue->process_queue[i].turnaround_time;
		total_wait_time = total_wait_time + queue->process_queue[i].wait_time;
		total_response_time = total_response_time + queue->process_queue[i].response_time;
		processed++;
	}
      }
	float avg_TT = total_turnaround_time/queue->size;
	float avg_WT = total_wait_time/queue->size;
	float avg_RT = total_response_time/queue->size;
	printf("\n\nAverage turnaround time: %.4f", avg_TT);
	printf("\nAverage waiting time: %.4f", avg_WT);
	printf("\nAverage response time: %.4f\n\n", avg_RT);
   }
   else
      puts("Queue was NULL");
}

void print_queue_fresh(struct ProcessQueue * queue) {

   if(queue != NULL){
      printf("%-2s%14s%14s%10s%11s%15s%17s%17s\n\n", "ID", "Arrival Time", "Service Time", "Priority", "Wait Time", "Response Time", "Turnaround Time", "Cumulative Time");
      int i;
      for(i = 0; i < queue->size; i++) {
         _print_process(queue->process_queue[i]);
      }
   }
   else
      puts("Queue was NULL");
}


void sort_by_remaining(struct ProcessQueue *queue){
    struct Process *processes = queue->process_queue;
    int index_of_earliest;
    struct Process swap;
    int i, j;
    for(i = 0; i < queue->size; i++) {
        index_of_earliest = i;
        for(j = i; j < queue->size; j++) {
            if(processes[j].service_time-processes[j].cumulative_time<
               processes[index_of_earliest].service_time- processes[index_of_earliest].cumulative_time) {
                index_of_earliest = j;
            }
        }
        swap = processes[i];
        processes[i] = processes[index_of_earliest];
        processes[index_of_earliest] = swap;
    }
    
}








