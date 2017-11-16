#include <stdlib.h>
#include <stdio.h>
#include "Process.h"

void print_process(struct Process process) {
   printf("%-2d%14d%14d%10d%11d%15d%17d%17d\n", process.id, process.arrival_time, process.service_time, process.priority, process.wait_time, process.response_time, process.turnaround_time, process.cumulative_time);
}
