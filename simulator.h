/*****************************************************************************/
/*  COMP30023 Assignment 1                                                   */
/*  Eishitha Galpayage Don                                                   */
/*  993413                                                                   */
/*  Simulator Header                                                         */
/*****************************************************************************/
#include "structs.h"

/*  Simulates executing the CPUs for 1 second  */
/*  Returns 0 if all CPUs idle, 1 otherwise */
int run_cpus(CPU_t **CPUs, int num_processors, int time, pqueue_t *started, pqueue_t *finished);

/*  Prints the started/resumed process to output file   */
void print_started_processes(pqueue_t *started, int time);

/*  Prints finished processes to output and calculates statistics   */
void finish_processes(pqueue_t *finished, int time, double *total_turnaround, 
                    double *max_overhead, double *total_overhead, int *num_proc_left);

/*  Initialises simulation variables, returning pointer to array of CPUs    */
CPU_t **init_simulation(int num_processors);

/*  Frees variables used in simulation  */
void kill_simulation(CPU_t **CPUs, int num_processors, pqueue_t *all_processes,
                    pqueue_t *incoming_processes, pqueue_t *finished, pqueue_t *started);

/*  Print performance statistics to 2 d.p.  */
void print_statistics(double total_overhead, double max_overhead, double total_turnaround, 
                    int num_proc_total, int time);

/*  Main simulation loop    */
void run_simulation(int num_processors, pqueue_t *all_processes, 
                    void (*scheduler)(pqueue_t *, CPU_t **, int));
