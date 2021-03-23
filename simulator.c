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
#include "pqueue.h"
#include "simulator.h"
#include "shortest_time.h"

void print_process(void *data){
    process_t *process = (process_t *)data;
    printf("process id: %d, arrival time: %d, remaining: %d, parallel?: %d \n",
            process->pid, process->arrival_time, process->remaining_time,
            process->parallel);
}

/*  Simulates executing the CPUs for 1 second  */
/*  Returns 1 if all CPUs idle, 0 otherwise */
int run_cpus(CPU_t **CPUs, int num_processors, int time, double *total_turnaround,
            double *max_overhead, double *total_overhead){
    // create queues to hold the started/finished processes, making it easier to 
    // print them afterwards
    pqueue_t *finished = create_pqueue();
    pqueue_t *started = create_pqueue();

    int idle = 1;
    for (int i = 0; i < num_processors; i++){
        if (CPUs[i]->total_remaining_time == 0){
            // CPU is idle
            continue;
        }
        idle = 0;

        // run process, replacing if needed
        if (CPUs[i]->current_process != pq_peek(CPUs[i]->process_queue)){   
            CPUs[i]->current_process = pq_peek(CPUs[i]->process_queue);
            pq_enqueue(started, CPUs[i]->current_process, 1, i);
        }
        CPUs[i]->current_process->remaining_time--;
        CPUs[i]->total_remaining_time--;
        
        // update priority of current process
        update(CPUs[i]->process_queue, 1, CPUs[i]->current_process->remaining_time, 0);

        // finish process, removing from process queue
        if (CPUs[i]->current_process->remaining_time == 0){
            pq_enqueue(finished, pq_dequeue(CPUs[i]->process_queue), 1, i);
        }
    }
    
    finish_processes(finished, time+1, CPUs, num_processors, total_turnaround,
                    max_overhead, total_overhead);
    print_started_processes(started, time);
    pq_free_queue(finished);
    pq_free_queue(started);
    return idle;
}

/*  Prints the started/resumed process to output file   */
void print_started_processes(pqueue_t *started, int time){
    process_t *process;
    while ((process = (process_t *)pq_dequeue(started))){
        printf("%d,RUNNING,pid=%d,remaining_time=%d,cpu=%d\n", time, 
                process->pid, process->remaining_time+1, 0);
    }
}

/*  Calculates the number of remaining processes across all CPUs    */
int num_proc_left(CPU_t **CPUs, int num_processors){

    int num = 0;
    for (int i = 0; i < num_processors; i++){
        num += get_length(CPUs[i]->process_queue);
        if (CPUs[i]->current_process){
            num++;
        }
    }
    return num;
}
/*  Prints finished processes to output and calculates statistics   */
void finish_processes(pqueue_t *finished, int time, CPU_t **CPUs, int num_processors, 
                    double *total_turnaround, double *max_overhead, double *total_overhead){

    int num = num_proc_left(CPUs, num_processors);
    double turnaround;
    double overhead;
    
    process_t *process;
    while ((process = (process_t *)pq_dequeue(finished))){
        turnaround = time - process->arrival_time;
        overhead = turnaround/process->exec_time;

        if (overhead > *max_overhead){
            *max_overhead = overhead;
        }

        *total_turnaround += turnaround;
        *total_overhead += overhead;

        printf("%d,FINISHED,pid=%d,proc_remaining=%d\n", time, process->pid,
                 num);
        free(process); 
    }
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
void run_simulation(int num_processors, pqueue_t *incoming_processes){

    // initialise simulation and create CPUs
    int time = -1, idle = 0;
    int num_processes = get_length(incoming_processes);
    double max_overhead = 0, total_overhead = 0, total_turnaround = 0;
    CPU_t **CPUs = init_simulation(num_processors);

    // simulate process scheduling/execution until no more processes
    process_t *next_process = pq_dequeue(incoming_processes);
    while(next_process || !idle){
        time++;

        // schedule incoming processes
        while(next_process && next_process->arrival_time <= time){
            shortest_time_remaining(next_process, CPUs, num_processors);
            next_process = pq_dequeue(incoming_processes);
        }
        idle = run_cpus(CPUs, num_processors, time, &total_turnaround,
                        &max_overhead, &total_overhead);
    }
    kill_simulation(CPUs, num_processors);
    pq_free_queue(incoming_processes);

    // print stats to 2 d.p.
    printf("Turnaround time %d\n", (int)ceil(total_turnaround/num_processes));
    printf("Time overhead %.2lf %.2lf\n", round(max_overhead*100)/100, 
                                        round(total_overhead*100/num_processes)/100);
    printf("Makespan %d\n", time);
}


