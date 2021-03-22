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


void print_process(void *data){
    process_t *process = (process_t *)data;
    printf("process id: %d, arrival time: %d, remaining: %d, parallel?: %d \n",
            process->pid, process->arrival_time, process->remaining_time,
            process->parallel);
}

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
        process->remaining_time = exec_time;
        process->parallel = (parallel == 'p');

        pq_enqueue(queue, process, process->arrival_time);
    }

    return queue;
}

int main(int argc, char *argv[]){

    /*  Parse command line args */
    int option;
    char *filename = NULL;
    int num_processors;
    while ((option = getopt(argc, argv, "f:p:c")) != -1){
        switch (option){
            case 'f':
                filename = optarg;
                break;
            case 'p':
                num_processors = atoi(optarg);
                break;
            case 'c':
                break;
            case '?':
                return 1;
        }
    }

    pqueue_t *queue = read_lines(filename);

    return 0;
}

