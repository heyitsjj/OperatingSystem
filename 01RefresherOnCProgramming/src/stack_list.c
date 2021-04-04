#include <stdlib.h> 
#include <stdio.h> 
#include <list.h>
#include <stack.h>

struct list_type *l; 
int count=0; 

/* Initialization of the stack */ 
int init_stack(){  
    l = malloc(sizeof(struct list_type));
    init_list(l); 
    count = 0; 
    return count; 
}

/* Insertion: adds an element to the top of the stack */ 
int push(void* element){  
    insert_head(l, element); 
    count++; 
    return count; 
}

/* Extraction: the element at the top of the stack is removed and its value is returned */ 
void* pop(){ 
    void* result = NULL; 
    result = extract_head(l); 
    count--; 
    return result; 
}

/* Returns the number of elements in the stack */ 
int size(){ 
    return count; 
}