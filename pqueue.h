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

/*  Add an item to priority queue                                           */
/*  Items can have multiple priority dimensions                             */
/*  num_priorities is the number of such dimensions                         */
/*  Subsequent args are the priorities, with the most important ones first  */
void pq_enqueue(pqueue_t *queue, void *data, int num_priorities, ...);

/*  Remove an item from front of queue and returns a pointer to the item    */
void *pq_dequeue(pqueue_t *queue);

/*  Prints all items in the queue using a given print function  */
void pq_print_queue(pqueue_t *queue, void (*print)(void *data));

/*  Updates the priority of an item in the queue    */
void update(pqueue_t *queue, int index, int priority, int dim);

/*  Frees all memory associated with a queue    */
void pq_free_queue(pqueue_t *queue);

/*  Repeatedly performs upheap operations on an item until it is at its     */
/*  highest allowed position with respect to the given priority dimension   */
void upheap(pqueue_t *queue, int i, int dim);

/*  Repeatedly performs downheap operations on an item until it is at its   */
/*  lowest allowed position with respect to the given priority dimension    */
void downheap(pqueue_t *queue, int i, int dim);

/*  Returns the (existing) index of node with lower priority with respect   */
/*  to the given priority dimension                                         */
int min_node(pqueue_t *queue, int i, int j, int dim);

/*  Returns number of items in queue    */
int get_length(pqueue_t *queue);

/*  Returns the head of queue without removing it   */
void *pq_peek(pqueue_t *queue);