/*****************************************************************************/
/*  COMP30023 Assignment 1                                                   */
/*  Eishitha Galpayage Don                                                   */
/*  993413                                                                   */
/*  Simulator Header                                                         */
/*****************************************************************************/
#include "pqueue.h"
typedef struct process process_t;
typedef struct CPU CPU_t;

struct process{
    int arrival_time;
    int pid;
    int exec_time;
    int remaining_time;
    int parallel;
};

struct CPU{
    int CPU_no;
    pqueue_t *process_queue;
    process_t *current_process;
    int total_remaining_time; 
};

/*  Simulates executing the CPUs for 1 second  */
/*  Returns 0 if all CPUs idle, 1 otherwise */
int run_cpus(CPU_t **CPUs, int num_processors, int time, double *total_turnaround,
            double *max_overhead, double *total_overhead);

/*  Prints the started/resumed process to output file   */
void print_started_processes(pqueue_t *started, int time);

/*  Calculates the number of remaining processes across all CPUs    */
int num_proc_left(CPU_t **CPUs, int num_processors);

/*  Prints finished processes to output and calculates statistics   */
void finish_processes(pqueue_t *finished, int time, CPU_t **CPUs, int num_processors, 
                    double *total_turnaround, double *max_overhead, double *total_overhead);

/*  Initialises simulation variables, returning pointer to array of CPUs    */
CPU_t **init_simulation(int num_processors);

/*  Frees variables used in simulation  */
void kill_simulation(CPU_t **CPUs, int num_processors);

/*  Main simulation loop    */
void run_simulation(int num_processors, pqueue_t *incoming_processes);
