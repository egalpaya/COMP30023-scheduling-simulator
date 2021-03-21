/*****************************************************************************/
/*  Implementation of a linked-list based queue                              */
/*  Eishitha Galpayage Don                                                   */
/*  993413                                                                   */
/*****************************************************************************/
#include <stdlib.h>
#include <assert.h>
#include "queue.h"

struct queue{
    node_t *head;
    node_t *foot;
    int length;
};

struct queue_node{
    void *data;
    node_t *next;
};
/*  Create an empty queue   */
queue_t *create_queue(){
    queue_t *queue = (queue_t *)malloc(sizeof(queue_t));
    assert(queue);
    queue->head = NULL;
    queue->foot = NULL;
    queue->length = 0;
    return queue;
}

/*  Add an item to back of queue    */
void enqueue(queue_t *queue, void *data){
    node_t *node = (node_t *)malloc(sizeof(node_t));
    assert(node);
    node->data = data;
    node->next = NULL;

    if (queue->foot){
        queue->foot->next = node;
    }
    queue->foot = node;
    queue->length++;
}

/*  Remove an item from front of queue
    Returns a pointer to the item       */
void *dequeue(queue_t *queue){
    if (queue->head == NULL){
        return NULL;
    }
    node_t *head = queue->head;
    queue->head = head->next;
    queue->length--;
    void *data = head->data;
    free(head);
    return data;
}

/*  Prints all items in the queue using a given print function  */
void print_queue(queue_t *queue, void (*print)(void *data)){
    node_t *head = queue->head;
    while (head){
        print(head->data);
        head = head->next;
    }
}

/*  Frees all memory associated with a queue    */
void free_queue(queue_t *queue){
    node_t *curr = queue->head;
    node_t *prev = NULL;
    while(curr){
        prev = curr;
        curr = curr->next;
        free(prev);
    }
    free(queue);
}