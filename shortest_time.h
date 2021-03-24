/*****************************************************************************/
/*  COMP30023 Assignment 1                                                   */
/*  Eishitha Galpayage Don                                                   */
/*  993413                                                                   */
/*  Shortest Time Remaining Algorithm Header                                 */
/*****************************************************************************/
#include "structs.h"

/*  Schedules incoming processes and assigns them to CPUs using shortest    */
/*  time remaining algorithm                                                */
void shortest_time_remaining(pqueue_t *incoming_processes, CPU_t **CPUs,
                            int num_processors);

/*  Returns a priority queue of the incoming processes, sorted by remaining */
/*  time and then by pid (pseudo heapsort in a way...)                      */
pqueue_t *sort_processes(pqueue_t *incoming_processes);

/*  Returns the minimum of 2 numbers    */
int find_min(int a, int b);