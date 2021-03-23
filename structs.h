/*****************************************************************************/
/*  COMP30023 Assignment 1                                                   */
/*  Eishitha Galpayage Don                                                   */
/*  993413                                                                   */
/*  Structs Header, defining common structs used throughout program          */
/*****************************************************************************/
#ifndef STRUCTS_H
#define STRUCTS_H

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

#endif