/*****************************************************************************/
/*  COMP30023 Assignment 1                                                   */
/*  Eishitha Galpayage Don                                                   */
/*  993413                                                                   */
/*  Process header                                                           */
/*****************************************************************************/
#include "structs.h"

/*  Create a process with the given characteristics */
process_t *create_process(int arrival_time, int pid, int exec_time, char parallel);

/*  Create a subprocess from the given parent   */
process_t *create_subprocess(process_t *parent, int arrival_time, int exec_time, 
                            int subprocess_no);