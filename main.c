/*****************************************************************************/
/*  COMP30023 Assignment 1                                                   */
/*  Eishitha Galpayage Don                                                   */
/*  993413                                                                   */
/*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "structs.h"
#include "process.h"
#include "simulator.h"
#include "shortest_time.h"



/*  Reads input, converts lines to process structs, and adds them to a queue */
/*  Returns the queue   */
pqueue_t *read_lines(char filename[]){

    FILE *file = fopen(filename, "r");
    assert(file);

    pqueue_t *queue = create_pqueue();

    int time, pid, exec_time;
    char parallel;
    while((fscanf(file, "%d %d %d %c", &time, &pid, &exec_time, &parallel)) == 4){

        process_t *process = create_process(time, pid, exec_time, parallel);
        pq_enqueue(queue, process, 2, time, pid);
    }
    
    fclose(file);
    return queue;
}

/*  Parse command line args */
void parse_args(int argc, char **argv, char **filename, int *num_processors, 
                void (**scheduler)(pqueue_t *, CPU_t **, int)){

    int option;
    while ((option = getopt(argc, argv, "f:p:c")) != -1){
        switch (option){
            case 'f':
                *filename = optarg;
                break;
            case 'p':
                *num_processors = atoi(optarg);
                if (*num_processors == 2){
                    *scheduler = shortest_time_remaining_2p;
                }
                break;
            case 'c':
                *scheduler = better_scheduler;
                break;
            case '?':
                exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char **argv){

    // parse args
    int num_processors;
    char *filename = NULL;
    void (*scheduler)(pqueue_t *, CPU_t **, int) = shortest_time_remaining_np;
    parse_args(argc, argv, &filename, &num_processors, &scheduler);

    // read in all processes
    pqueue_t *all_processes = read_lines(filename);

    // start simulation
    run_simulation(num_processors, all_processes, (*scheduler));
    
    return 0;
}

