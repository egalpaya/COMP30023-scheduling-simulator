/*****************************************************************************/
/*  COMP30023 Assignment 1                                                   */
/*  Eishitha Galpayage Don                                                   */
/*  993413                                                                   */
/*  Min Priority Queue Implementation Code                                   */
/*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include "pqueue.h"

#define INITIAL_SIZE 2

struct pqueue{
    pnode_t **nodes;
    int size;
    int num_items;
};

struct pqueue_node{
    void *data;
    int *priorities;
    int priority_dims;
};

/*  Create an empty queue   */
pqueue_t *create_pqueue(){

    pqueue_t *q = (pqueue_t *)malloc(sizeof(pqueue_t));
    assert(q);

    q->num_items = 0;
    q->size = INITIAL_SIZE;
    q->nodes = (pnode_t **)malloc(sizeof(pnode_t *)*INITIAL_SIZE);
    assert(q->nodes);

    return q;
}

/*  Add an item to priority queue                                           */
/*  Items can have multiple priority dimensions                             */
/*  num_priorities is the number of such dimensions                         */
/*  Subsequent args are the priorities, with the most important ones first  */
void pq_enqueue(pqueue_t *queue, void *data, int num_priorities, ...){

    // -1 is to account for the empty 0 index, as we start at 1
    if (queue->num_items == queue->size - 1){
        queue->size = (queue->size)*2;
        queue->nodes = (pnode_t **)realloc(queue->nodes, 
                                            (queue->size)*sizeof(pnode_t));
        assert(queue->nodes);
    }

    pnode_t *node = (pnode_t *)malloc(sizeof(pnode_t));
    assert(node);

    node->data = data;

    
    node->priority_dims = num_priorities;
    node->priorities = (int *)malloc(sizeof(int)*num_priorities);
    assert(node->priorities);

    // load the array of priorities using stdarg.h macros
    va_list ap;
    va_start(ap, num_priorities);
    for(int i = 0; i < num_priorities; i++){
        node->priorities[i] = va_arg(ap, int);
    }
    va_end(ap);

    // pre-increment starts indexing at 1 for easier upheap/downheaps
    queue->nodes[++(queue->num_items)] = node;
    upheap(queue, queue->num_items, 0);
}

/*  Remove an item from front of queue and returns a pointer to the item    */
void *pq_dequeue(pqueue_t *queue){

    if (queue->num_items == 0){
        return NULL;
    }

    pnode_t *node = queue->nodes[1];

    // swap with last element and downheap
    queue->nodes[1] = queue->nodes[queue->num_items--];
    downheap(queue, 1, 0);

    void *data = node->data;
    free(node->priorities);
    free(node);

    return data;
}

/*  Prints all items in the queue using a given print function  */
void pq_print_queue(pqueue_t *queue, void (*print)(void *data)){

    printf("Printing queue...\n");
    for (int i = 1; i <= queue->num_items; i++){
        print(queue->nodes[i]->data);
    }
    printf("\n"); 
}

/*  Updates the priority of an item in the queue in the given dimension */
void update(pqueue_t *queue, int index, int priority, int dim){

    queue->nodes[index]->priorities[dim] = priority;

    // only one of the below executes, the other will do nothing
    upheap(queue, index, 0);
    downheap(queue, index, 0);
}

/*  Frees all memory associated with a queue    */
void pq_free_queue(pqueue_t *queue){

    for (int i = 1; i <= queue->num_items; i++){
        free(queue->nodes[i]->priorities);
        free(queue->nodes[i]);
    }

    free(queue->nodes);
    free(queue);
}

/*  Repeatedly performs upheap operations on an item until it is at its     */
/*  highest allowed position with respect to the given priority dimension   */
void upheap(pqueue_t *queue, int i, int dim){
    
    // if the root is reached, or all given priorities are equal, return
    if (i == 1 || dim >= queue->nodes[i]->priority_dims){
        return;
    }

    pnode_t *tmp;

    // swap node with parent while parent has a greater priority
    if (queue->nodes[i/2]->priorities[dim] > queue->nodes[i]->priorities[dim]){
        tmp = queue->nodes[i];
        queue->nodes[i] = queue->nodes[i/2];
        queue->nodes[i/2] = tmp;

        // recursively call on parent node, resetting the dimension to 0
        upheap(queue, i/2, 0);
    } else if (queue->nodes[i/2]->priorities[dim] == queue->nodes[i]->priorities[dim]){
        // current priority dimension is the same, so check the next one
        upheap(queue, i, dim+1);
    }
}

/*  Repeatedly performs downheap operations on an item until it is at its   */
/*  lowest allowed position with respect to the given priority dimension    */
void downheap(pqueue_t *queue, int i, int dim){

    // if index is already a leaf or or all given priorities are equal, return
    if (i > (queue->num_items)/2 || dim >= queue->nodes[i]->priority_dims){
        return;
    }

    pnode_t *node = queue->nodes[i];
    pnode_t *tmp;
    
    // Get the smallest child 
    int min_idx = min_node(queue, 2*i, 2*i + 1, 0);
    pnode_t *min_node = queue->nodes[min_idx];
    
    if (node->priorities[dim] > min_node->priorities[dim]){
        // swap with smallest child
        tmp = queue->nodes[i];
        queue->nodes[i] = queue->nodes[min_idx];
        queue->nodes[min_idx] = tmp;

        // recursively call on the child, resetting the dimension to 0
        downheap(queue, min_idx, 0);
    } else if (node->priorities[dim] == min_node->priorities[dim]) {
        // current priority dimension is the same, so check the next one
        downheap(queue, i, dim+1);
    }
}

/*  Returns the (existing) index of node with lower priority    */
int min_node(pqueue_t *queue, int i, int j, int dim){
    
    if (j > queue->num_items){
        return i;
    }

    // if there are no more priority dimensions to check, return i
    if (dim >= queue->nodes[i]->priority_dims || dim >= queue->nodes[j]->priority_dims){
        return i;
    }

    if (queue->nodes[i]->priorities[dim] == queue->nodes[j]->priorities[dim]){
        // if nodes are equal in priority, check next priority dimension
        return min_node(queue, i, j, dim+1);
    } else if (queue->nodes[i]->priorities[dim] > queue->nodes[j]->priorities[dim]){
        return j;
    } else {
        return i;
    }
}

/*  Returns number of items in queue    */
int get_length(pqueue_t *queue){

    return queue->num_items;
}

/*  Returns the head of queue without removing it   */
void *pq_peek(pqueue_t *queue){
    
    if (queue->num_items == 0){
        return NULL;
    } else {
        return queue->nodes[1]->data;
    } 
}

/*  Returns the priority of a node at the given index and dimension */
int get_priority(pqueue_t *queue, int index, int dim){

    if (queue->nodes[index]){
        return queue->nodes[index]->priorities[dim];
    } else {
        return -1;
    }
}