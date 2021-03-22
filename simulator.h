/*****************************************************************************/
/*  COMP30023 Assignment 1                                                   */
/*  Eishitha Galpayage Don                                                   */
/*  993413                                                                   */
/*  Simulator Header                                                         */
/*****************************************************************************/

typedef struct process process_t;
typedef struct CPU CPU_t;

struct process{
    int arrival_time;
    int pid;
    int remaining_time;
    int parallel;
};

/*  Simulates executing the CPUs for 1 second  */
void run_cpus(CPU_t **CPUs, int num_processors, int time);

/*  Prints the started/resumed process to output file   */
void print_started_processes(pqueue_t started, int time,);

/*  Calculates the number of remaining processes across all CPUs    */
int num_proc_left(CPU_t **CPUS, int num_processors);

/*  Prints finished process to output   */
void print_finished_processes(pqueue_t finished, int time, CPU_t **CPUs, int num_processors);

/*  Initialises simulation variables, returning pointer to array of CPUs    */
CPU_t **init_simulation(int num_processors);

/*  Frees variables used in simulation  */
void kill_simulation(CPU_t **CPUs, int num_processors);

/*  Main simulation loop    */
void run_simulation(int num_processors, pqueue_t incoming_processes);
