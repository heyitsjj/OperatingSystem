#include <stdlib.h> 
#include <stdio.h> 
#include <list.h>
#include <fifo.h>


struct list_type *dqp; 
int count=0; 

/* Initialization of the queue */
int init_queue(){
    dqp = malloc(sizeof(struct list_type));
    init_list(dqp); 
    count = 0; 
    return count; 
}

/* Insertion of a new element.
 The new element is added to the head of the queue. */
int queue(void* element){
    insert_head(dqp, element); 
    count++; 
    return count;
}

/* Extraction of the element at the tail of the queue.
 Returns (also deletes) that element.  */
void* dequeue(){
    void* result = NULL; 
    result = extract_tail(dqp); 
    count--; 
    return result; 
}

/* Returns the number of elements in the queue */
int size(){ 
    return count; 
}