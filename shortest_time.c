/*****************************************************************************/
/*  COMP30023 Assignment 1                                                   */
/*  Eishitha Galpayage Don                                                   */
/*  993413                                                                   */
/*  Shortest Time Remaining Algorithm Code                                   */
/*****************************************************************************/
#include <stdlib.h>
#include <limits.h>
#include "shortest_time.h"

/*  Schedules incoming processes and assigns them to CPUs using shortest    */
/*  time remaining algorithm                                                */
void shortest_time_remaining(pqueue_t *incoming_processes, CPU_t **CPUs, 
                            int num_processors){
    
    pqueue_t *sorted_processes = sort_processes(incoming_processes);
    process_t *process = NULL;

    // assign all processes to CPUs
    while ((process = pq_dequeue(sorted_processes))){
        if (process->parallel){

        } else {
            // find CPU with least remaning execution time and assign
            int fastest = 0, time_remaining = INT_MAX;
            for(int i = 0; i < num_processors; i++){
                if (CPUs[i]->total_remaining_time < time_remaining){
                    fastest = i;
                    time_remaining = CPUs[i]->total_remaining_time;
                }
            }

            // assign to the CPU
            pq_enqueue(CPUs[fastest]->process_queue, process, 2, process->remaining_time,
                        process->pid);
            CPUs[fastest]->total_remaining_time += process->remaining_time;
        }   
    }
}

/*  Returns a priority queue of the incoming processes, sorted by remaining */
/*  time and then by pid (pseudo heapsort in a way...)                      */
pqueue_t *sort_processes(pqueue_t *incoming_processes){

    pqueue_t *sorted_processes = create_pqueue();
    process_t *curr = NULL;
    while ((curr = pq_dequeue(incoming_processes))){
        pq_enqueue(sorted_processes, curr, 2, curr->remaining_time, curr->pid);
    }

    return sorted_processes;
}