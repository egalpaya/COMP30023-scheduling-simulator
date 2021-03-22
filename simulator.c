/*****************************************************************************/
/*  COMP30023 Assignment 1                                                   */
/*  Eishitha Galpayage Don                                                   */
/*  993413                                                                   */
/*  Simulator Code                                                           */
/*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "pqueue.h"
#include "simulator.h"

struct CPU{
    int CPU_no;
    pqueue_t *process_queue;
    process_t *current_process;
    int total_remaining_time; 
};

/*  Simulates executing the CPUs for 1 second  */
void run_cpus(CPU_t **CPUs, int num_processors, int time){
    // create queues to hold the started/finished processes, making it easier to 
    // print them afterwards
    pqueue_t *finished = create_pqueue();
    pqueue_t *started = create_pqueue();

    for (int i = 0; i < num_processors; i++){
        if (CPUs[i]->total_remaining_time == 0){
            // CPU is idle
            return;
        }

        CPUs[i]->current_process->remaining_time--;
        CPUs[i]->total_remaining_time--;

        if (CPUs[i]->current_process->remaining_time == 0){
            pq_enqueue(finished, CPUs[i]->current_process, i)
            CPUs[i]->current_process = pq_dequeue(CPUs[i]->process_queue);
            pq_enqueue(started, CPUs[i]->current_process, i);
        }
    }
    
    print_finished_processes(finished, time, CPUs, num_processors);
    print_started_processes(started, time);
}

/*  Prints the started/resumed process to output file   */
void print_started_processes(pqueue_t started, int time,){
    process_t process;
    while (process = (process_t *)dequeue(started)){
        printf("%d,RUNNING,pid=%d,remaining_time=%d,cpu=%d\n", time, 
                process->pid, process->remaining_time, 0);
    }
}

/*  Calculates the number of remaining processes across all CPUs    */
int num_proc_left(CPU_t **CPUS, int num_processors){
    int num = 0;
    for (int i = 0; i < num_processors; i++){
        num += CPUs[i]->process_queue->num_items;
        if (CPUs[i]->current_process){
            num++;
        }
    }
    return num;
}
/*  Prints finished process to output   */
void print_finished_processes(pqueue_t finished, int time, CPU_t **CPUs, int num_processors){
    int num_proc_left = num_proc_left(CPUS, num_processors);
    
    process_t process = (process_t *)dequeue(finished);
    while (process = (process_t *)dequeue(finished)){
        printf("%d,FINISHED,pid=%d,proc_remaining=%d\n", time, process->pid,
                 num_proc_left);
    }
};

/*  Initialises simulation variables, returning pointer to array of CPUs    */
CPU_t **init_simulation(int num_processors){
    CPU_t *CPUs = (CPU_t **)malloc(sizeof(CPU_t*)*num_processors);
    assert(CPUs);

    for (int i = 0; i < num_processors; i++){
        CPU_t CPUs[i] = (CPU_t *)malloc(sizeof(CPU_t));
        assert(CPUs[i]);

        CPUs[i]->CPU_no = i;
        CPUs[i]->process_queue = create_pqueue();
        CPUs[i]->current_process = NULL;
        CPUs[i]->total_remaining_time = 0; 
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
void run_simulation(int num_processors, pqueue_t incoming_processes){

    // initialise simulation and create CPUs
    int time = 0;
    CPU_t **CPUs = init_simulation(num_processors);

    // simulate process scheduling/execution until no more processes
    process_t *next_process = dequeue(incoming_processes);
    while(next_process){
        // schedule incoming processes
        while(next_process && next_process->arrival_time <= time){
            schedule(next_process, CPUs);
            next_process = dequeue(incoming_processes)
        }

        // run CPUs
        for (int i = 0; i < num_processors; i++){
            run_cpu(CPUs[i])
        }
        
        time++;
    }

    kill_simulation(CPUs, num_processors);
}

