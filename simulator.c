/*****************************************************************************/
/*  COMP30023 Assignment 1                                                   */
/*  Eishitha Galpayage Don                                                   */
/*  993413                                                                   */
/*  Simulator Code                                                           */
/*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "simulator.h"
#include "shortest_time.h"

/*  Simulates executing the CPUs for 1 second  */
/*  Returns 1 if all CPUs idle, 0 otherwise */
int run_cpus(CPU_t **CPUs, int num_processors, int time, pqueue_t *started, pqueue_t *finished){

    int idle = 1;
    for (int i = 0; i < num_processors; i++){

        if (!pq_peek(CPUs[i]->process_queue)){
            // CPU is idle
            continue;
        }

        idle = 0;

        // replace current process if new one has higher priority
        if (CPUs[i]->current_process != pq_peek(CPUs[i]->process_queue)){   
            CPUs[i]->current_process = pq_peek(CPUs[i]->process_queue);
            pq_enqueue(started, CPUs[i]->current_process, 1, i);
        }

        // "run" the process
        CPUs[i]->current_process->remaining_time--;
        CPUs[i]->total_remaining_time--;
        
        // update priority of current process
        // MAYBE MOVE TO SHORTEST TIME ALGORITHM FUNCTION
        update(CPUs[i]->process_queue, 1, CPUs[i]->current_process->remaining_time, 0);

        // finish process, removing from process queue
        if (CPUs[i]->current_process->remaining_time == 0){
            pq_enqueue(finished, pq_dequeue(CPUs[i]->process_queue), 1, i);
            CPUs[i]->current_process = NULL;
        }
    }

    return idle;
}

/*  Prints the started/resumed process to output file   */
void print_started_processes(pqueue_t *started, int time){

    process_t *process;
    while (get_length(started) > 0){
        // we previously assigned the CPU no. as the priority in the queue
        int CPU_no = get_priority(started, 1, 0);
        process = (process_t *)pq_dequeue(started);

        // print subprocess no. if process has a parent
        if (process->parent){
            printf("%d,RUNNING,pid=%d.%d,remaining_time=%d,cpu=%d\n", time, 
                process->pid, process->subprocess_no, process->remaining_time+1, CPU_no);
        } else {
            printf("%d,RUNNING,pid=%d,remaining_time=%d,cpu=%d\n", time, 
                process->pid, process->remaining_time+1, CPU_no);
        }
    }
}

/*  Prints finished processes to output and calculates statistics   */
void finish_processes(pqueue_t *finished, int time, double *total_turnaround, 
                    double *max_overhead, double *total_overhead, int *num_proc_left){

    int num_finished = 0;
    double turnaround;
    double overhead;

    pqueue_t *processes = create_pqueue();
    process_t *process;

    // first loop calculates statistics and number of finished parent processes, adding 
    // finished processes to a queue
    int i = 0;
    while ((process = (process_t *)pq_dequeue(finished))){

        // if the process is a subprocess, check if the parent process has remaining subprocesses
        // if it does, decrement the counter and move on. if not, finish the parent process
        if (process->parent){

            process->parent->remaining_subprocesses--;

            if (process->parent->remaining_subprocesses != 0){
                free(process);
                continue;
            } else {
                process_t *tmp = process;
                process = process->parent;
                free(tmp);
            }
        }

        turnaround = time - process->arrival_time;
        overhead = turnaround/process->exec_time;

        if (overhead > *max_overhead){
            *max_overhead = overhead;
        }

        *total_turnaround += turnaround;
        *total_overhead += overhead;

        num_finished++;

        pq_enqueue(processes, process, 1, i++);
    }

    *num_proc_left -= num_finished;

    // second loop to print all the finished parent processes
    while((process = (process_t *)pq_dequeue(processes))){

        printf("%d,FINISHED,pid=%d,proc_remaining=%d\n", time, process->pid, *num_proc_left);
        free(process); 
    }

    pq_free_queue(processes);
}

/*  Initialises simulation variables, returning pointer to array of CPUs    */
CPU_t **init_simulation(int num_processors){
    CPU_t **CPUs = (CPU_t **)malloc(sizeof(CPU_t*)*num_processors);
    assert(CPUs);

    for (int i = 0; i < num_processors; i++){
        CPU_t *CPU = (CPU_t *)malloc(sizeof(CPU_t));
        assert(CPU);

        CPU->CPU_no = i;
        CPU->process_queue = create_pqueue();
        CPU->current_process = NULL;
        CPU->total_remaining_time = 0; 
        CPUs[i] = CPU;
    }

    return CPUs;
}

/*  Frees variables used in simulation  */
void kill_simulation(CPU_t **CPUs, int num_processors){
    for (int i = 0; i < num_processors; i++){
        pq_free_queue(CPUs[i]->process_queue);
        free(CPUs[i]);
    }

    free(CPUs);
}

/*  Main simulation loop    */
void run_simulation(int num_processors, pqueue_t *all_processes){

    // initialise simulation and create CPUs
    int time = -1, idle = 0;
    int num_proc_total = get_length(all_processes);
    int num_proc_left = 0;
    double max_overhead = 0, total_overhead = 0, total_turnaround = 0;
    CPU_t **CPUs = init_simulation(num_processors);

    // create queues to hold started and finished processes
    pqueue_t *started = create_pqueue();
    pqueue_t *finished = create_pqueue();

    // simulate process scheduling/execution until no more processes
    process_t *next_process = NULL;
    pqueue_t *incoming_processes = create_pqueue();

    while(next_process || !idle){
        time++;

        // get all processes coming in at current time
        while((next_process = (process_t *)pq_peek(all_processes)) && 
                next_process->arrival_time == time){

            // remove from all processes queue and add to incoming processes
            pq_enqueue(incoming_processes, pq_dequeue(all_processes), 1, time);
            num_proc_left++;
        }

        // schedule incoming processes
        shortest_time_remaining(incoming_processes, CPUs, num_processors);

        // run the cpus
        idle = run_cpus(CPUs, num_processors, time, started, finished);
            
        print_started_processes(started, time);
        finish_processes(finished, time+1, &total_turnaround, &max_overhead, &total_overhead,
                        &num_proc_left);
    }
    
    kill_simulation(CPUs, num_processors);
    pq_free_queue(all_processes);
    pq_free_queue(incoming_processes);
    pq_free_queue(finished);
    pq_free_queue(started);

    // print stats to 2 d.p.
    printf("Turnaround time %d\n", (int)ceil(total_turnaround/num_proc_total));
    printf("Time overhead %.2lf %.2lf\n", round(max_overhead*100)/100, 
                                        round(total_overhead*100/num_proc_total)/100);
    printf("Makespan %d\n", time);
}


