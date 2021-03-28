/*****************************************************************************/
/*  COMP30023 Assignment 1                                                   */
/*  Eishitha Galpayage Don                                                   */
/*  993413                                                                   */
/*  Shortest Time Remaining Algorithm Header                                 */
/*****************************************************************************/
#ifndef SCHEDULERS_H
#define SCHEDULERS_H

#include "structs.h"

/*  Schedules incoming processes and assigns them to 2 CPUs using shortest time             */
/*  remaining algorithm (num_processors argument exists for function pointer compatibility) */
void shortest_time_remaining_2p(pqueue_t *incoming_processes, CPU_t **CPUs,
                            int num_processors);

/*  Schedules incoming processes and assigns them to n CPUs using shortest  */
/*  time remaining algorithm                                                */
void shortest_time_remaining_np(pqueue_t *incoming_processes, CPU_t **CPUs,
                            int num_processors);

/*  Improved scheduling algorithm - uses different k and greedy number partitioning     */ 
void better_scheduler(pqueue_t *incoming_processes, CPU_t **CPUs, int num_processors);

/*  Returns the index of CPU with least remaining execution time    */
int find_fastest_CPU(CPU_t **CPUs, int num_processors);

/*  Returns a priority queue of the incoming processes, sorted by remaining */
/*  time and then by pid (pseudo heapsort in a way...)                      */
pqueue_t *sort_processes(pqueue_t *incoming_processes);

/*  Returns a priority queue of the incoming processes, sorted by remaining */
/*  time (descending) and then by pid (pseudo heapsort in a way...)         */
pqueue_t *sort_processes_reverse(pqueue_t *incoming_processes);

/*  Returns the minimum of 2 numbers    */
int find_min(int a, int b);

/*  Updates priorities of processes to reflect remaining time   */
void update_priorities(CPU_t **CPUs, int num_processors);

/*  Finds the longest remaining CPU time across all CPUs    */
int find_longest_CPU_time(CPU_t **CPUs, int num_processors);

/*  Alternative algorithm to determine number of subprocesses   */
int find_num_subprocesses(int num_processors, int exec_time);

/*  Calculates the length of each subprocess, given a total exec time and no. subprocesses  */
int get_subprocess_length(int exec_time, int num_subprocesses);

/*  Assigns a process to a given CPU    */
void assign_to_CPU(CPU_t **CPUs, int CPU_no, process_t *process);

#endif