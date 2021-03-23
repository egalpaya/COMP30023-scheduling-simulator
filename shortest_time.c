/*****************************************************************************/
/*  COMP30023 Assignment 1                                                   */
/*  Eishitha Galpayage Don                                                   */
/*  993413                                                                   */
/*  Shortest Time Remaining Algorithm Code                                   */
/*****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "simulator.h"

void shortest_time_remaining(process_t *process, CPU_t **CPUs, int num_processors){
    for(int i = 0; i < num_processors; i++){
        pq_enqueue(CPUs[i]->process_queue, process, 2, process->remaining_time,
                    process->pid);
        CPUs[i]->total_remaining_time += process->remaining_time;
    }
}