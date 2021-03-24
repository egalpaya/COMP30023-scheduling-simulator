/*****************************************************************************/
/*  COMP30023 Assignment 1                                                   */
/*  Eishitha Galpayage Don                                                   */
/*  993413                                                                   */
/*  Process code, implements functions related to processes                  */
/*****************************************************************************/
#include <stdlib.h>
#include <assert.h>
#include "structs.h"


/*  Create a process with the given characteristics */
process_t *create_process(int arrival_time, int pid, int exec_time, char parallel){

    process_t *process = (process_t *)malloc(sizeof(process_t));
    assert(process);

    process->arrival_time = arrival_time;
    process->pid = pid;
    process->exec_time = exec_time;
    process->remaining_time = exec_time;
    process->parallel = (parallel == 'p');
    // indicate it is not a subprocess
    process->subprocess_no = -1;
    process->parent = NULL;
    process->remaining_subprocesses = 0;

    return process;
}

/*  Create a subprocess from the given parent   */
process_t *create_subprocess(process_t *parent, int arrival_time, int exec_time, 
                            int subprocess_no){
     
    process_t *process = (process_t *)malloc(sizeof(process_t));
    assert(process);

    process->arrival_time = arrival_time;
    process->pid = parent->pid;
    process->exec_time = exec_time;
    process->remaining_time = exec_time;
    // assuming subprocesses cannot be further parallelised
    process->parallel = 0; 
    process->subprocess_no = subprocess_no;
    process->parent = parent;

    // increment parent's subprocess counter
    parent->remaining_subprocesses++;

    return process;
}