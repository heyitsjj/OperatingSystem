#include <os-memory.h>


void first_fit(int size, freezone* fz) {
    int found = freelist;
    int previous = -1;

    while ((found != -1) && (heap[found] < size)) {
        previous = found;
        found = heap[found + 1];
    }

    fz->previous = previous;
    fz->found = found;
}

void best_fit(int size, freezone* fz) {
    // TODO
    int found = freelist; 
    int temp = found; 
    int result = found; 
    int previous = -1; 
    int smallestSizeDifference; 


    /* 
       Find the first available freezone (zone size is bigger than or equal to size)
    */
    smallestSizeDifference = heap[found] - size; 
    temp = heap[found+1]; 
    while((smallestSizeDifference < 0) && (temp != -1)){ 
        smallestSizeDifference = heap[temp] - size;
        previous = temp; 
        temp = heap[temp+1]; 
        found = temp; 
    }


    /* 
       Compare the first available freezone with other freezone to 
       find out which one is the best 
     */
    result = found; 
    while((found != -1) && (temp != -1)) {  
        int checkSize = heap[found+1]; 

        // If there is no more next available freezone, break the while loop by 
        // setting temp to -1 
        if (checkSize == -1){ 
            temp = -1; 
        }

        int currentSizeDifference = heap[checkSize] - size; 

        if ((currentSizeDifference >= 0) && (currentSizeDifference <= smallestSizeDifference)) { 
            smallestSizeDifference = currentSizeDifference; 
            previous = found;
            found = checkSize; 
            result = found; 
        } else { 
            int tempStore = found; 
            found = heap[found+1];
            previous = tempStore; 
        }
    }
    fz->previous = previous;
    fz->found = result;
}

void worst_fit(int size, freezone* fz) {
    // TODO
    int found = freelist; 
    int temp = found; 
    int result = found; 
    int previous = -1; 
    int largestSizeDifference; 


    /* 
       Find the first available freezone (zone size is bigger than or equal to size)
    */
    largestSizeDifference = heap[found] - size; 
    temp = heap[found+1]; 
    while((largestSizeDifference < 0) && (temp != -1)){ 
        largestSizeDifference = heap[temp] - size;
        previous = temp; 
        temp = heap[temp+1]; 
        found = temp; 
    }


    /* 
       Compare the first available freezone with other freezone to 
       find out which one is the best 
     */
    result = found; 
    while((found != -1) && (temp != -1)) {  
        int checkSize = heap[found+1]; 

        // If there is no more next available freezone, break the while loop by 
        // setting temp to -1 
        if (checkSize == -1){ 
            temp = -1; 
        }

        int currentSizeDifference = heap[checkSize] - size; 

        if ((currentSizeDifference >= 0) && (currentSizeDifference >= largestSizeDifference)) { 
            largestSizeDifference = currentSizeDifference; 
            previous = found;
            found = checkSize; 
            result = found; 
        } else { 
            int tempStore = found; 
            found = heap[found+1];
            previous = tempStore; 
        }
    }
    fz->previous = previous;
    fz->found = result;
}


void* heap_malloc(int size) {
    freezone result;                    //free zone found for the allocation
    int allocation_size = size + 1;     //size of the allocated zone
    void *ptr = NULL;                   //pointer to the allocated zone
    
    find_free_zone(size, &result);
    
    //Cannot find a free zone
    if (result.found == -1)
        return NULL;
    
    // TODO
    ptr = &heap[result.found+1]; 
    if (heap[result.found+1] == -1){ 
        heap[result.found + allocation_size] = heap[result.found] - allocation_size;
        heap[result.found + allocation_size + 1] = heap[result.found + 1]; 
        heap[result.found] = size;
        if (freelist == result.found){ 
            freelist = result.found + allocation_size; 
        } else { 
            heap[freelist+1]=result.found + allocation_size;
            //freelist = heap[result.found+1];
        }
    } else { 
        if (heap[result.found] == size){ 
            if (freelist >= result.found){ 
                freelist = heap[result.found+1]; 
            } else{ 
                heap[result.previous + 1] = heap[result.found + 1];
            }
        } else { 
            heap[result.found + allocation_size] = heap[result.found] - allocation_size;
            heap[result.found + allocation_size + 1] = heap[result.found + 1]; 
            heap[result.found] = size;
            if (freelist == result.found){ 
                freelist = result.found + allocation_size; 
            } else { 
                heap[result.previous + 1] = result.found + allocation_size;
            }
        }
    }
    return ptr;
}


int heap_free(void *dz) {
    // TODO
    int indexOfDelete;
    int found = freelist; 
    int previous = -1; 

    indexOfDelete = ptr2ind(dz) - 1; 
    /*  When the only freezone is the remaining freezone area at the end of the heap 
        In other words, there is only 1 freezone */
    if ((heap[freelist+1] == -1) && (found != -1)){ 
        // when the last fz and the dz are next to each other 
        if ((found - indexOfDelete) == (heap[indexOfDelete]+1)){ 
            heap[indexOfDelete] = heap[indexOfDelete] + heap[found] + 1; 
            heap[indexOfDelete + 1] = previous; 
            freelist = indexOfDelete; 
        } else{  
        // when the last fz and the dz are not next to each other 
        // in other words, we are now freeing an area in the middle of the heap 
            heap[indexOfDelete + 1] = found; 
            previous = freelist; 
            freelist = indexOfDelete; 
            found = freelist; 
        }
    } else if ((heap[freelist+1] != -1) && (found != -1)){ 
    /* When there are more than 1 freezone in the heap  */
        if (((found - indexOfDelete) > 0) && ((found - indexOfDelete) == (heap[indexOfDelete]+1))){ 
            // when there is 1 freezone right after the dz 
            heap[indexOfDelete] = heap[indexOfDelete] + heap[found] + 1; 
            heap[indexOfDelete+1] = heap[found+1]; 
            freelist = indexOfDelete; 
        } else if (((found - indexOfDelete) < 0) && ((indexOfDelete - found) == (heap[found]+1))){ 
            // when there is 1 freezone right before the dz 
            heap[found] = heap[found] + heap[indexOfDelete] + 1; 
        } else { 
            while (found != indexOfDelete){ 
                if (heap[found+1] < indexOfDelete){ 
                    previous = found; 
                    found = heap[found+1]; 
                } else if (heap[found+1] > indexOfDelete) {
                    heap[indexOfDelete+1] = heap[found+1]; 
                    heap[found+1] = indexOfDelete; 
                    found = indexOfDelete; 
                }
            }
        }
    }
    
    return 0;
}

int heap_defrag() {
    // TODO
    return 0;
}


