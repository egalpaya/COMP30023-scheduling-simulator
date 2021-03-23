/*****************************************************************************/
/*  COMP30023 Assignment 1                                                   */
/*  Eishitha Galpayage Don                                                   */
/*  993413                                                                   */
/*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include "pqueue.h"
#include "simulator.h"



/*  Reads input, converts lines to process structs, and adds them to a queue */
/*  Returns the queue   */
pqueue_t *read_lines(char filename[]){
    FILE *file = fopen(filename, "r");
    assert(file);

    pqueue_t *queue = create_pqueue();

    int time, pid, exec_time;
    char parallel;
    while((fscanf(file, "%d %d %d %c", &time, &pid, &exec_time, &parallel)) == 4){
        process_t *process = (process_t *)malloc(sizeof(process_t));
        assert(process);

        process->arrival_time = time;
        process->pid = pid;
        process->exec_time = exec_time;
        process->remaining_time = exec_time;
        process->parallel = (parallel == 'p');

        pq_enqueue(queue, process, 2, process->arrival_time, process->pid);
    }
    
    fclose(file);
    return queue;
}

/*  Parse command line args */
void parse_args(int argc, char **argv, char **filename, int *num_processors){

    int option;
    while ((option = getopt(argc, argv, "f:p:c")) != -1){
        switch (option){
            case 'f':
                *filename = optarg;
                break;
            case 'p':
                *num_processors = atoi(optarg);
                break;
            case 'c':
                break;
            case '?':
                exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char **argv){

    int num_processors;
    char *filename = NULL;
    parse_args(argc, argv, &filename, &num_processors);

    pqueue_t *all_processes = read_lines(filename);
    run_simulation(num_processors, all_processes);
    
    return 0;
}

