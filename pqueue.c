/*****************************************************************************/
/*  COMP30023 Assignment 1                                                   */
/*  Eishitha Galpayage Don                                                   */
/*  993413                                                                   */
/*  Min Priority Queue Implementation Code                                   */
/*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "pqueue.h"

#define INITIAL_SIZE 2

struct pqueue{
    pnode_t **nodes;
    int size;
    int num_items;
};

struct pqueue_node{
    void *data;
    int priority;
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

/*  Add an item to priority queue    */
void pq_enqueue(pqueue_t *queue, void *data, int priority){
    if (queue->num_items == queue->size){
        queue->size = (queue->size)*2;
        queue->nodes = (pnode_t **)realloc(queue->nodes, 
                                            (queue->size)*sizeof(pnode_t));
        assert(queue->nodes);
    }

    pnode_t *node = (pnode_t *)malloc(sizeof(pnode_t));
    assert(node);
    node->data = data;
    node->priority = priority;

    // pre-increment starts indexing at 1 for easier upheap/downheaps
    queue->nodes[++(queue->num_items)] = node;
    upheap(queue, queue->num_items);
}

/*  Remove an item from front of queue
    Returns a pointer to the item       */
void *pq_dequeue(pqueue_t *queue){
    if (queue->num_items == 0){
        return NULL;
    }

    pnode_t *node = queue->nodes[1];

    // swap with last element
    queue->nodes[1] = queue->nodes[queue->num_items--];

    downheap(queue, 1);

    void *data = node->data;
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

/*  Updates the priority of an item in the queue    */
void update(pqueue_t *queue, int index, int priority){
    queue->nodes[index]->priority = priority;

    // as upheap and downheap check necessary conditions, only one executes
    upheap(queue, index);
    downheap(queue, index);
}

/*  Frees all memory associated with a queue    */
void pq_free_queue(pqueue_t *queue){
    for (int i = 1; i <= queue->num_items; i++){
        free(queue->nodes[i]);
    }
    free(queue->nodes);
    free(queue);
}

/*  Repeatedly performs upheap operations on an item until it is at its */
/*  highest allowed position    */
void upheap(pqueue_t *queue, int i){
    
    if (i == 1){
        return;
    }

    pnode_t *tmp;

    // Swap node with parent while parent has a greater priority
    if (queue->nodes[i/2]->priority > queue->nodes[i]->priority){
        tmp = queue->nodes[i];
        queue->nodes[i] = queue->nodes[i/2];
        queue->nodes[i/2] = tmp;

        // Recursively call on parent node
        upheap(queue, i/2);
    }
}

/*  Repeatedly performs downheap operations on an item until it is at its */
/*  lowest allowed position    */
void downheap(pqueue_t *queue, int i){
    // Index is already a leaf node
    if (i > (queue->num_items - 1)/2){
        return;
    }

    pnode_t *node = queue->nodes[i];
    pnode_t *tmp;
    
    // Get the smallest child 
    int min_idx = minNode(queue, 2*i, 2*i + 1);
    pnode_t *min_node = queue->nodes[min_idx];
    
    if (node->priority > min_node->priority){
        // Swap with smallest child
        tmp = queue->nodes[i];
        queue->nodes[i] = queue->nodes[min_idx];
        queue->nodes[min_idx] = tmp;

        // Recursively call on the child
        downheap(queue, min_idx);
    }
}

/* Returns the (existing) index of node with lower priority */
int minNode(pqueue_t *queue, int i, int j){

    if (i >= queue->size){
        return j;
    } else if (j >= queue->size){
        return i;
    } else if (queue->nodes[i]->priority > queue->nodes[j]->priority){
        return j;
    } else {
        return i;
    }
}