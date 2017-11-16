#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <ctype.h>
#include "ProcessQueue.h"

//waiting time = sum of all intervals when it is waiting. turnaround time - service time
//service time = service time
// turnaround time = waiting time + service time or end - arrival time
// response time = time before it was touched first time

void RoundRobinAlgorithm(struct ProcessQueue* queue)
{
     struct ProcessQueue* workingQueue = make_queue(size(queue));
     struct ProcessQueue* finalQueue = make_queue(size(queue));
     struct Process* next = peek(queue);
     int time = next->arrival_time;
     enqueue(workingQueue, next);
     print_queue(queue);
     dequeue(queue);
     int maxIdle = 0;
     int counter = 0;
     while(!is_empty(queue) || !is_empty(workingQueue))
    {
        //if there is no work, wait
        bool needEnq = false;
        struct Process* temp;
        if(!is_empty(workingQueue))
        {
            counter = 0;
            temp = dequeue(workingQueue);
            temp->cumulative_time++;

        // if a process hasn't been touched
            if(temp->response_time == -1)
            {
                temp->response_time = (time + 1) - temp->arrival_time;
            }
        //we have just finished the process in this quantum iteration
            if(temp->cumulative_time == temp->service_time)
            {
                temp->turnaround_time = (time + 1) - temp->arrival_time;
                temp->wait_time = temp->turnaround_time - temp->service_time;
                enqueue(finalQueue, temp);
            }
            else
            {
                needEnq = true;
            }
        }
        else
        {
            counter++;
            if(counter > maxIdle)
                maxIdle = counter;
        }
        while(!is_empty(queue) && (peek(queue)->arrival_time <= (time + 1)))
        {
            if((peek(queue)->response_time == -1) && (time + size(workingQueue) > 99))
            {
                //this will not add not ran processes
                //struct Process* bad = peek(queue);
                //enqueue(finalQueue, bad);
                dequeue(queue);
            }
            else
            {
                struct Process *proc = dequeue(queue);
                enqueue(workingQueue, proc);
            }
        }
        if(needEnq == true)
        {
            enqueue(workingQueue, temp);
        }
        time++;
    }




    //printf("\n\n\n\n\n");
    //printf("MAX IDLE TIME IS %d\n\n", maxIdle);
    print_queue(finalQueue);
    destroy_queue(workingQueue);
    destroy_queue(finalQueue);
}

