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

/*  Schedules incoming processes and assigns them to 2 CPUs using shortest  */
/*  time remaining algorithm                                                */
void shortest_time_remaining_2p(pqueue_t *incoming_processes, CPU_t **CPUs, 
                                int num_processors){

    // update priorities of existing processes
    update_priorities(CPUs, 2);
    
    // sort incoming processes by remaining time
    pqueue_t *sorted_processes = sort_processes(incoming_processes);
    process_t *process = NULL;

    // assign all processes to CPUs
    while ((process = pq_dequeue(sorted_processes))){

        if (process->parallel){

            double num_subprocesses = find_min(process->exec_time, 2);
            int exec_time = ceil(process->exec_time/num_subprocesses) + 1;

            for (int i = 0; i < num_subprocesses; i++){
                process_t *subprocess = create_subprocess(process, process->arrival_time,
                                                        exec_time, i);
                // insert back into process queue
                pq_enqueue(CPUs[i]->process_queue, subprocess, 2, subprocess->exec_time,
                            subprocess->pid);

            }

        } else {

            // assign to fastest CPU
            int fastest = find_fastest_CPU(CPUs, 2);
            pq_enqueue(CPUs[fastest]->process_queue, process, 2, process->remaining_time,
                        process->pid);
            CPUs[fastest]->total_remaining_time += process->remaining_time;
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
            pq_enqueue(CPUs[fastest]->process_queue, process, 2, process->remaining_time,
                        process->pid);
            CPUs[fastest]->total_remaining_time += process->remaining_time;
        }   
    }

    pq_free_queue(sorted_processes);
}

/*  Improved scheduling algorithm */ 
void better_scheduler2(pqueue_t *incoming_processes, CPU_t **CPUs, int num_processors){

    update_priorities(CPUs, num_processors);

    // create queue to sort processes by remaining time (descending)
    pqueue_t *sorted_processes = create_pqueue();
    process_t *process = NULL;

    // create auxiliary queue to store parallelisable processes
    pqueue_t *parallel = create_pqueue();

    // split and sort the non-parallelisable and parallelisable processes
    while ((process = pq_dequeue(incoming_processes))){

        if (process->parallel){
            pq_enqueue(parallel, process, 2, -(process->remaining_time), process->pid);
        } else {
            pq_enqueue(sorted_processes, process, 2, -(process->remaining_time), process->pid);
        }
    }
    

    // assign non parallelisable processes to fastest CPUs
    while ((process = pq_dequeue(sorted_processes))){

        int fastest = find_fastest_CPU(CPUs, num_processors);
        pq_enqueue(CPUs[fastest]->process_queue, process, 2, -(process->remaining_time),
                    process->pid);
        CPUs[fastest]->total_remaining_time += process->remaining_time;
    }

    // split parallelisable processes into subprocesses
    while ((process = pq_dequeue(parallel))){

        //double num_subprocesses = find_num_subprocesses(CPUs, num_processors, process->exec_time);
        double num_subprocesses = find_min(process->exec_time, num_processors);
        int exec_time = ceil(process->exec_time/num_subprocesses) + 1;

        for (int i = 0; i < num_subprocesses; i++){
            process_t *subproc = create_subprocess(process, process->arrival_time, exec_time, i);

            // insert back into sorted process queue by execution time (descending)
            pq_enqueue(sorted_processes, subproc, 3, -(subproc->exec_time),
                        subproc->pid, subproc->subprocess_no);
        }
    }

    // assign parallelisable processes to fastest CPUs
    while ((process = pq_dequeue(sorted_processes))){

        int fastest = find_fastest_CPU(CPUs, num_processors);
        pq_enqueue(CPUs[fastest]->process_queue, process, 2, -(process->remaining_time),
                    process->pid);
        CPUs[fastest]->total_remaining_time += process->remaining_time;
    }

    // update priorities of existing processes
    update_priorities(CPUs, num_processors);
    pq_free_queue(sorted_processes);
    pq_free_queue(parallel);
}

/*  Alternative algorithm to determine number of subprocesses   */
int find_num_subprocesses(CPU_t **CPUs, int num_processors, int exec_time){
    
    pqueue_t *sorted = sort_CPUs(CPUs, num_processors);
    CPU_t *curr = NULL;
    CPU_t *next = pq_dequeue(sorted);

    int num_subprocesses = 1;

    for (int i = 0; i < num_processors - 1; i++){
        curr = next;
        next = pq_dequeue(sorted);
        int subprocess_length = get_subprocess_length(exec_time, num_subprocesses);

        if (next->total_remaining_time - curr->total_remaining_time < subprocess_length){
            num_subprocesses++;
        } else {
            return num_subprocesses;
        }
        
    }
    return num_subprocesses;

}

/*  Returns a priority queue of CPUs, ordered by total remaining time, lowest first  */
pqueue_t *sort_CPUs(CPU_t **CPUs, int num_processors){

    pqueue_t *sorted = create_pqueue();

    for (int i = 0; i < num_processors; i++){
        pq_enqueue(sorted, CPUs[i], 1, CPUs[i]->total_remaining_time);
    }

    return sorted;
}

/*  Calculates the length of each subprocess, given a total exec time and no. subprocesses  */
int get_subprocess_length(int exec_time, int num_subprocesses){

    if (num_subprocesses == 1){
        return exec_time;
    } else {
        return ceil((double)exec_time/num_subprocesses) + 1;
    }
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


/*  Extracts all processes from all CPUs and inserts them into sorted queue   */
void extract_processes(CPU_t **CPUs, int num_processors, pqueue_t *sorted_processes){

    process_t *curr = NULL;
    for (int i = 0; i < num_processors; i++){
        while ((curr = pq_dequeue(CPUs[i]->process_queue))){
            pq_enqueue(sorted_processes, curr, 2, -(curr->remaining_time), curr->pid);
        }
        CPUs[i]->total_remaining_time = 0;
    }

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

/*  Updates priorities of processes to reflect remaining time   */
void update_priorities_v2(CPU_t **CPUs, int num_processors){

    for (int i = 0; i < num_processors; i++){
        
        // update priority of current process if it exists
        if (CPUs[i]->current_process){
            update(CPUs[i]->process_queue, 1, CPUs[i]->current_process->remaining_time, 1);
        }
    }
}


/*  Improved scheduling algorithm */ 
void better_scheduler(pqueue_t *incoming_processes, CPU_t **CPUs, int num_processors){

        // update priorities of existing processes
    update_priorities(CPUs, num_processors);
    
    // sort incoming processes by remaining time (descending)
    pqueue_t *sorted_processes = sort_processes_reverse(incoming_processes);
    process_t *process = NULL;

    // assign all processes to CPUs
    process = pq_dequeue(sorted_processes);
    while (process){

        // assign to fastest CPU
        int fastest = find_fastest_CPU(CPUs, num_processors);
        int longest_time = find_longest_CPU_time(CPUs, num_processors);

        while (process && CPUs[fastest]->total_remaining_time <= longest_time){
            pq_enqueue(CPUs[fastest]->process_queue, process, 2, process->remaining_time,
                    process->pid);
            CPUs[fastest]->total_remaining_time += process->remaining_time; 
            process = pq_dequeue(sorted_processes);
        } 
    }

    pq_free_queue(sorted_processes);
}

int find_longest_CPU_time(CPU_t **CPUs, int num_processors){

    int longest_time = 0;
    for (int i = 0; i < num_processors; i++){
        if (CPUs[i]->total_remaining_time > longest_time){
            longest_time = CPUs[i]->total_remaining_time;
        }
    }

    return longest_time;
}
