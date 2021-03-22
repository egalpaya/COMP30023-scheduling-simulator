/*****************************************************************************/
/*  COMP30023 Assignment 1                                                   */
/*  Eishitha Galpayage Don                                                   */
/*  993413                                                                   */
/*  Min Priority Queue Implementation Header                                 */
/*****************************************************************************/
typedef struct pqueue pqueue_t;
typedef struct pqueue_node pnode_t;

/*  Create an empty queue   */
pqueue_t *create_pqueue();

/*  Add an item to priority queue    */
void pq_enqueue(pqueue_t *queue, void *data, int priority);

/*  Remove an item from front of queue
    Returns a pointer to the item       */
void *pq_dequeue(pqueue_t *queue);

/*  Prints all items in the queue using a given print function  */
void pq_print_queue(pqueue_t *queue, void (*print)(void *data));

/*  Updates the priority of an item in the queue    */
void update(pqueue_t *queue, int index, int priority);

/*  Frees all memory associated with a queue    */
void pq_free_queue(pqueue_t *queue);

/*  Repeatedly performs upheap operations on an item until it is at its */
/*  highest allowed position    */
void upheap(pqueue_t *queue, int index);

/*  Repeatedly performs downheap operations on an item until it is at its */
/*  lowest allowed position    */
void downheap(pqueue_t *queue, int index);

/* Returns the (existing) index of node with lower priority */
int minNode(pqueue_t *queue, int i, int j);