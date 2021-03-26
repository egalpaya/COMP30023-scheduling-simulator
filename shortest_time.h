/*****************************************************************************/
/*  COMP30023 Assignment 1                                                   */
/*  Eishitha Galpayage Don                                                   */
/*  993413                                                                   */
/*  Shortest Time Remaining Algorithm Header                                 */
/*****************************************************************************/
#ifndef SCHEDULERS_H
#define SCHEDULERS_H

#include "structs.h"

/*  Schedules incoming processes and assigns them to CPUs using shortest    */
/*  time remaining algorithm                                                */
void shortest_time_remaining(pqueue_t *incoming_processes, CPU_t **CPUs,
                            int num_processors);

/*  Returns a priority queue of the incoming processes, sorted by remaining */
/*  time and then by pid (pseudo heapsort in a way...)                      */
pqueue_t *sort_processes(pqueue_t *incoming_processes);

/*  Returns the minimum of 2 numbers    */
int find_min(int a, int b);

/*  Improved scheduling algorithm   */ 
void better_scheduler(pqueue_t *incoming_processes, CPU_t **CPUs, int num_processors);

/*  Alternative algorithm to determine number of subprocesses   */
int find_num_subprocesses(CPU_t **CPUs, int num_processors, int exec_time);

/*  Returns a priority queue of CPUs, ordered by total remaining time, lowest first  */
pqueue_t *sort_CPUs(CPU_t **CPUs, int num_processors);

/*  Calculates the length of each subprocess, given a total exec time and no. subprocesses  */
int get_subprocess_length(int exec_time, int num_subprocesses);

/*  Returns the index of CPU with least remaining execution time    */
int find_fastest_CPU(CPU_t **CPUs, int num_processors);


pqueue_t *sort_processes2(pqueue_t *incoming_processes);

#endif