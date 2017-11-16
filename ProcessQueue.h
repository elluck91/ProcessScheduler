#ifndef PROCESSQUEUE_H
#define PROCESSQUEUE_H
#include <stdbool.h>
#include "Process.h"

struct ProcessQueue{
   int max_size;
   int size;
   int last;
   struct Process *process_queue;
};

struct ProcessQueue * make_queue(int size);
void destroy_queue(struct ProcessQueue *queue);
int size(struct ProcessQueue *queue);
struct Process * peek(struct ProcessQueue *queue);
bool is_full(struct ProcessQueue *queue);
bool is_empty(struct ProcessQueue *queue);
struct Process * dequeue(struct ProcessQueue *queue);
bool enqueue(struct ProcessQueue *queue, struct Process *process);
void sort_by_arrival_time(struct ProcessQueue *queue);
void sort_by_service_time(struct ProcessQueue *queue);
void sort_by_priority(struct ProcessQueue *queue);
void sort_by_remaining(struct ProcessQueue *queue);
struct Process * _normalize(struct ProcessQueue *queue);
struct ProcessQueue * generate_process_queue(int size, int seed);
void _print_process(struct Process process);
void print_queue(struct ProcessQueue * queue); 
void print_queue_fresh(struct ProcessQueue * queue);
#endif
