#include <stdlib.h>
#include <stdio.h>
#include "Constants.h"
#include "ProcessQueue.h"
#include "HPFP.h"

void processHPFP(struct ProcessQueue *queue) {
	// We are using four queues for four priority levels
	// Assuming we generate 100 processes, I am ensuring that
	// all procesess fit into any priority queue.
    struct ProcessQueue *priorityOne = make_queue(100);
    struct ProcessQueue *priorityTwo = make_queue(100);
    struct ProcessQueue *priorityThree = make_queue(100);
    struct ProcessQueue *priorityFour = make_queue(100);

	// this queue stores all processes that have started before quanta 99
    struct ProcessQueue *finishedProcesses = make_queue(100);
    puts("Starting processing:\n\n");

    int i;
    int maxIdleTime = 0;
    int tempIdleTime = 0;
    int currentTime = 0;
    int quePointer = 0;

	// empty proccess used for checking
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

		// Processing priority 4 queue
        if(priorityFour->size > 0) {
            //puts("Selected Queue 4\n");
            tempIdleTime = 0;
            currentProcess = dequeue(priorityFour);
            process(currentProcess, currentTime);
            if (currentProcess->service_time == currentProcess->cumulative_time)
                enqueue(finishedProcesses, currentProcess);
            else
                enqueue(priorityFour, currentProcess);
        }
		// processing priority 3 queue
        else if(priorityThree->size > 0) {
            //puts("Selected Queue 3\n");
            tempIdleTime = 0;
            currentProcess = dequeue(priorityThree);
            process(currentProcess, currentTime);
            if (currentProcess->service_time == currentProcess->cumulative_time)
                enqueue(finishedProcesses, currentProcess);
            else
                enqueue(priorityThree, currentProcess);
        }
		// processing priority 2 queue
        else if(priorityTwo->size > 0) {
            //puts("Selected Queue 2\n");
            tempIdleTime = 0;
            currentProcess = dequeue(priorityTwo);
            process(currentProcess, currentTime);
            if (currentProcess->service_time == currentProcess->cumulative_time)
                enqueue(finishedProcesses, currentProcess);
            else
                enqueue(priorityTwo, currentProcess);
        }
		// processig priority 1 queue
        else if(priorityOne->size > 0) {
            //puts("Selected Queue 1\n");
            tempIdleTime = 0;
            currentProcess = dequeue(priorityOne);
            process(currentProcess, currentTime);
            if (currentProcess->service_time == currentProcess->cumulative_time)
                enqueue(finishedProcesses, currentProcess);
            else
                enqueue(priorityOne, currentProcess);
        }
		// if all queues are empty this is idle time
        else {
            //puts("All queues empty\n");
            tempIdleTime++;
            if (tempIdleTime > maxIdleTime)
                maxIdleTime = tempIdleTime;
        }
        currentTime++;
    }
	// Now the time has reached quanta 99
	// We do not add any more processes to the queues
	// Only finish those that started before quanta 99
    while(priorityFour->size > 0) {
        currentProcess = dequeue(priorityFour);
        if(currentProcess->remaining_time == currentProcess->service_time) {
            continue;
        }
        else {
            process(currentProcess, currentTime);
            if (currentProcess->service_time == currentProcess->cumulative_time)
                enqueue(finishedProcesses, currentProcess);
            else
                enqueue(priorityOne, currentProcess);
            currentTime++;
        }
    }

    while(priorityThree->size > 0) {
        currentProcess = dequeue(priorityThree);
        if(currentProcess->remaining_time == currentProcess->service_time) {
            continue;
        }
        else {
            process(currentProcess, currentTime);
            if (currentProcess->service_time == currentProcess->cumulative_time)
                enqueue(finishedProcesses, currentProcess);
            else
                enqueue(priorityThree, currentProcess);
            currentTime++;
        }
    }

    while(priorityTwo->size > 0) {
        currentProcess = dequeue(priorityTwo);
        if(currentProcess->remaining_time == currentProcess->service_time) {
            continue;
        }
        else {
            process(currentProcess, currentTime);
            if (currentProcess->service_time == currentProcess->cumulative_time)
                enqueue(finishedProcesses, currentProcess);
            else
                enqueue(priorityTwo, currentProcess);
            currentTime++;
        }
    }

    while(priorityOne->size > 0) {
        currentProcess = dequeue(priorityOne);
        if(currentProcess->remaining_time == currentProcess->service_time) {
            continue;
        }
        else {
            process(currentProcess, currentTime);
            if (currentProcess->service_time == currentProcess->cumulative_time)
                enqueue(finishedProcesses, currentProcess);
            else
                enqueue(priorityOne, currentProcess);
            currentTime++;
        }
    }

    //printf("Number of processes in finishedProcesses = %d\n", finishedProcesses->size);
    puts("\n");

    print_queue(finishedProcesses);

    printf("\nMax Idle Time: %d\n", maxIdleTime);
};

// Function processes the current processes as if it was a CPU
void process(struct Process *process, int currentTime) {
    printf("%d, ", process->id);
    //puts("IN PROCESS");
    if (process->remaining_time == process->service_time) {
        process->response_time = currentTime + 1 - process->arrival_time;
        process->cumulative_time = (process->cumulative_time) + 1;
        process->remaining_time = (process->remaining_time) - 1;
    }
    else {
        process->cumulative_time = (process->cumulative_time) + 1;
        process->remaining_time = (process->remaining_time) - 1;
    }

    //printf("Process id: %d | Priority: %d | ArrivalTime: %d | ServiceTime: %d | RemainingTime: %d\n",
    //    process->id, process->priority, process->arrival_time, process->service_time, process->remaining_time);

    if (process->remaining_time == 0) {
        process->turnaround_time = (currentTime + 1) - process->arrival_time;
        process->wait_time = process->turnaround_time - process->service_time;
        //printf("Process %d terminated.\n", process->id);
    }


};

