#include <stdlib.h>
#include <stdio.h>
#include "Constants.h"
#include "ProcessQueue.h"
#include "HPFNP.h"
#include "HPFP.h"
// Process Highest Priority First Non-Preemptive 
void processHPFNP(struct ProcessQueue *queue) {
// 4 priority queues for each priority level
struct ProcessQueue *priorityOne = make_queue(100);
    struct ProcessQueue *priorityTwo = make_queue(100);
    struct ProcessQueue *priorityThree = make_queue(100);
    struct ProcessQueue *priorityFour = make_queue(100);
	
	// final queue that stores processes that began by quanta 99
    struct ProcessQueue *finishedProcesses = make_queue(100);
    puts("Starting processing:\n\n");

    int i;
    int maxIdleTime = 0;
    int tempIdleTime = 0;
    int currentTime = 0;
    int lastProcessTime = 0;
    int quePointer = 0;
    struct Process *currentProcess = malloc(sizeof(struct Process));

    // This is simulation of CPU Time
    while(currentTime < MAX_QUANTA) {
        //printf("\n\nNext Slice, at CurrentTime = %d\n", currentTime);
        // Add newly arrived processes
        while(queue->process_queue[quePointer].arrival_time == currentTime) {
            switch (queue->process_queue[quePointer].priority) {
                case 4:
                    //puts("\nADDING TO PRIORITY 4 QUEUE\n");
                    enqueue(priorityFour, queue->process_queue + quePointer);
                    break;
                case 3:
                    //puts("\nADDING TO PRIORITY 3 QUEUE\n");
                    enqueue(priorityThree, queue->process_queue + quePointer);
                    break;
                case 2:
                    //puts("\nADDING TO PRIORITY 2 QUEUE\n");
                    enqueue(priorityTwo, queue->process_queue + quePointer);
                    break;
                case 1:
                    //puts("\nADDING TO PRIORITY 1 QUEUE\n");
                    enqueue(priorityOne, queue->process_queue + quePointer);
                    break;
            }

            quePointer++;
        }
        /*
        printf("Size of priority que 4: %d\n", priorityFour->size);
        printf("Size of priority que 3: %d\n", priorityThree->size);
        printf("Size of priority que 2: %d\n", priorityTwo->size);
        printf("Size of priority que 1: %d\n", priorityOne->size);
        */
        // Only dequeue new process if the previous one is done.
        if (currentProcess->remaining_time == 0) {
            if (currentProcess->service_time > 0) {
                enqueue(finishedProcesses, currentProcess);
                currentProcess = malloc(sizeof(struct Process));
            }

            if(priorityFour->size > 0) {
                //Selected Queue 4
                currentProcess = dequeue(priorityFour);
            }
            else if(priorityThree->size > 0) {
                //Selected Queue 3
                currentProcess = dequeue(priorityThree);
            }
            else if(priorityTwo->size > 0) {
                //Selected Queue 2
                currentProcess = dequeue(priorityTwo);
            }
            else if(priorityOne->size > 0) {
                //Selected Queue 1
                currentProcess = dequeue(priorityOne);
            }
            else {
                currentTime++;
                //All queues empty
                tempIdleTime++;
                if (tempIdleTime > maxIdleTime)
                    maxIdleTime = tempIdleTime;
            }
        }
        else {
            tempIdleTime = 0;
            process(currentProcess, currentTime);
            currentTime++;
        }
    }
	// Now we have added all processes that were supposed to
	// start before quanta 99, now we will only finish those
	// that have already started
    if(currentProcess->remaining_time == 0) {
        enqueue(finishedProcesses, currentProcess);
        int t;
        while(priorityFour->size > 0) {
            currentProcess = dequeue(priorityFour);
            if(currentProcess->remaining_time == currentProcess->service_time) {
                continue;
            }
            else {
                for(t = 0; t < currentProcess->remaining_time; t++) {
                    process(currentProcess, currentTime);
                    currentTime++;
                }
                enqueue(finishedProcesses, currentProcess);

            }

        }
        while(priorityThree->size > 0) {
            currentProcess = dequeue(priorityThree);
            if(currentProcess->remaining_time == currentProcess->service_time) {
                continue;
            }
            else {
                for(t = 0; t < currentProcess->remaining_time; t++) {
                    process(currentProcess, currentTime);
                    currentTime++;
                }
                enqueue(finishedProcesses, currentProcess);
            }

        }

        while(priorityTwo->size > 0) {
            currentProcess = dequeue(priorityTwo);
            if(currentProcess->remaining_time == currentProcess->service_time) {
                continue;
            }
            else {
                for(t = 0; t < currentProcess->remaining_time; t++) {
                    process(currentProcess, currentTime);
                    currentTime++;
                }
                enqueue(finishedProcesses, currentProcess);
            }

        }

        while(priorityOne->size > 0) {
            currentProcess = dequeue(priorityOne);
            if(currentProcess->remaining_time == currentProcess->service_time) {
                continue;
            }
            else {
                for(t = 0; t < currentProcess->remaining_time; t++) {
                    process(currentProcess, currentTime);
                    currentTime++;
                }
                enqueue(finishedProcesses, currentProcess);

            }

        }
    }




    //printf("Number of processes in finishedProcesses = %d\n", finishedProcesses->size);

    puts("\n");

    print_queue(finishedProcesses);

    printf("\nMax Idle Time: %d\n", maxIdleTime);
}
