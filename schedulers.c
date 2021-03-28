/*****************************************************************************/
/*  COMP30023 Assignment 1                                                   */
/*  Eishitha Galpayage Don                                                   */
/*  993413                                                                   */
/*  Shortest Time Remaining Algorithm Code                                   */
/*****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <assert.h>
#include "process.h"
#include "schedulers.h"

/*  Schedules incoming processes and assigns them to 2 CPUs using shortest time             */
/*  remaining algorithm (num_processors argument exists for function pointer compatibility) */
void shortest_time_remaining_2p(pqueue_t *incoming_processes, CPU_t **CPUs, 
                                int num_processors){

    // update priorities of existing processes
    update_priorities(CPUs, num_processors);
    
    // sort incoming processes by remaining time
    pqueue_t *sorted_processes = sort_processes(incoming_processes);
    process_t *process = NULL;

    // assign all processes to CPUs
    while ((process = pq_dequeue(sorted_processes))){

        if (process->parallel){

            double num_subprocesses = find_min(process->exec_time, num_processors);
            int exec_time = ceil(process->exec_time/num_subprocesses) + 1;

            for (int i = 0; i < num_subprocesses; i++){

                process_t *subprocess = create_subprocess(process, process->arrival_time,
                                                        exec_time, i);
                // insert into corresponding CPU's process queue
                assign_to_CPU(CPUs, i, subprocess);
            }

        } else {

            // assign to fastest CPU
            int fastest = find_fastest_CPU(CPUs, num_processors);
            assign_to_CPU(CPUs, fastest, process);
        }   
    }

    pq_free_queue(sorted_processes);
}

/*  Schedules incoming processes and assigns them to n CPUs using shortest  */
/*  time remaining algorithm                                                */
void shortest_time_remaining_np(pqueue_t *incoming_processes, CPU_t **CPUs, 
                            int num_processors){

    // update priorities of existing processes
    update_priorities(CPUs, num_processors);
    
    // sort incoming processes by remaining time
    pqueue_t *sorted_processes = sort_processes(incoming_processes);
    process_t *process = NULL;

    // assign all processes to CPUs
    while ((process = pq_dequeue(sorted_processes))){

        if (process->parallel){

            double num_subprocesses = find_min(process->exec_time, num_processors);
            int exec_time = ceil(process->exec_time/num_subprocesses) + 1;

            for (int i = 0; i < num_subprocesses; i++){

                process_t *subprocess = create_subprocess(process, process->arrival_time,
                                                        exec_time, i);
                // insert back into process queue
                pq_enqueue(sorted_processes, subprocess, 3, subprocess->exec_time,
                            subprocess->pid, subprocess->subprocess_no);

            }

        } else {

            // assign to fastest CPU
            int fastest = find_fastest_CPU(CPUs, num_processors);
            assign_to_CPU(CPUs, fastest, process);
        }   
    }

    pq_free_queue(sorted_processes);
}

/*  Improved scheduling algorithm - uses different k and greedy number partitioning     */ 
void better_scheduler(pqueue_t *incoming_processes, CPU_t **CPUs, int num_processors){

    // update priorities of existing processes
    update_priorities(CPUs, num_processors);
    
    // sort incoming processes by remaining time (descending)
    pqueue_t *sorted_processes = sort_processes_reverse(incoming_processes);

    int parallel = 0;

    // assign all processes to CPUs
    process_t *process = pq_dequeue(sorted_processes);
    while (process){

        if (process->parallel){
            parallel = 1;

            double num_subprocesses = find_num_subprocesses(num_processors, process->exec_time);
            int exec_time = get_subprocess_length(process->exec_time, num_subprocesses);

            for (int i = 0; i < num_subprocesses; i++){
            
                process_t *subprocess = create_subprocess(process, process->arrival_time,
                                                        exec_time, i);
                // insert back into process queue
                pq_enqueue(sorted_processes, subprocess, 3, -(subprocess->exec_time),
                            subprocess->pid, subprocess->subprocess_no);

            }
            process = pq_dequeue(sorted_processes);
        }

        // find fastest CPU and the current longest remaining time for any CPU
        int fastest = find_fastest_CPU(CPUs, num_processors);
        int longest_time = find_longest_CPU_time(CPUs, num_processors);

        // if no processes have been parallelised, maximise the smallest CPU exection time
        if (!parallel){

            // keep assigning processes to the fastest CPU until it becomes the slowest/longest one
            while (process && CPUs[fastest]->total_remaining_time <= longest_time){

                assign_to_CPU(CPUs, fastest, process);
                process = pq_dequeue(sorted_processes);
            }
        } else {

            // if there are parallelised processes, assign to the CPU with smallest exec time but 
            // do not "fill it up", i.e recalculate fastest CPU for next process
            assign_to_CPU(CPUs, fastest, process);
            process = pq_dequeue(sorted_processes);
        }
    }

    pq_free_queue(sorted_processes);
}

/*  Returns the index of CPU with least remaining execution time    */
int find_fastest_CPU(CPU_t **CPUs, int num_processors){

    int fastest = 0, time_remaining = INT_MAX;

    for(int i = 0; i < num_processors; i++){

        if (CPUs[i]->total_remaining_time < time_remaining){
            fastest = i;
            time_remaining = CPUs[i]->total_remaining_time;
        }
    }

    return fastest;
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

/*  Returns a priority queue of the incoming processes, sorted by remaining */
/*  time (descending) and then by pid (pseudo heapsort in a way...)         */
pqueue_t *sort_processes_reverse(pqueue_t *incoming_processes){

    pqueue_t *sorted_processes = create_pqueue();
    process_t *curr = NULL;

    while ((curr = pq_dequeue(incoming_processes))){

        pq_enqueue(sorted_processes, curr, 3, curr->arrival_time, -(curr->remaining_time), 
                    curr->pid);
    }

    return sorted_processes;
}

/*  Returns the minimum of 2 numbers    */
int find_min(int a, int b){
    
    return (a > b) ? b : a;
}

/*  Updates priorities of processes to reflect remaining time   */
void update_priorities(CPU_t **CPUs, int num_processors){

    for (int i = 0; i < num_processors; i++){
        
        // update priority of current process if it exists
        if (CPUs[i]->current_process){
            update(CPUs[i]->process_queue, 1, CPUs[i]->current_process->remaining_time, 0);
        }
    }
}

/*  Finds the longest remaining CPU time across all CPUs    */
int find_longest_CPU_time(CPU_t **CPUs, int num_processors){

    int longest_time = 0;
    for (int i = 0; i < num_processors; i++){
        
        if (CPUs[i]->total_remaining_time > longest_time){
            longest_time = CPUs[i]->total_remaining_time;
        }
    }

    return longest_time;
}

/*  Alternative algorithm to determine number of subprocesses   */
int find_num_subprocesses(int num_processors, int exec_time){

    int num_subprocesses = 1;

    while (num_subprocesses < find_min(exec_time, num_processors)){

        int subprocess_length = get_subprocess_length(exec_time, num_subprocesses+1);
        
        // keep the combined length of all subprocesses less than parent process time + no. CPUs
        if ((num_subprocesses+1) * subprocess_length < exec_time + num_processors){
            num_subprocesses++;
        } else {
            break;
        }
    }

    return num_subprocesses;
}

/*  Calculates the length of each subprocess, given a total exec time and no. subprocesses  */
int get_subprocess_length(int exec_time, int num_subprocesses){

    if (num_subprocesses == 1){
        return exec_time;
    } else {
        return ceil((double)exec_time/num_subprocesses) + 1;
    }
}

/*  Assigns a process to a given CPU    */
void assign_to_CPU(CPU_t **CPUs, int CPU_no, process_t *process){

    pq_enqueue(CPUs[CPU_no]->process_queue, process, 2, process->remaining_time, process->pid);
    CPUs[CPU_no]->total_remaining_time += process->remaining_time; 
}