typedef struct queue queue_t;
typedef struct queue_node node_t;

/*  Create an empty queue   */
queue_t *create_queue();

/*  Add an item to back of queue    */
void enqueue(queue_t *queue, void *data);

/*  Remove an item from front of queue
    Returns a pointer to the item       */
void *dequeue(queue_t *queue);

/*  Prints all items in the queue using a given print function  */
void print_queue(queue_t *queue, void (*print)(void *data));

/*  Frees all memory associated with a queue    */
void free_queue(queue_t *queue);