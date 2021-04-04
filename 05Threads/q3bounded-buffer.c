#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <pthread.h> 
#include <semaphore.h>

#ifndef MAX_CAP 
#define MAX_CAP 10
#endif 

#ifndef NUM_PROD 
#define NUM_PROD 5
#endif 

#ifndef NUM_CONS 
#define NUM_CONS 5
#endif 

#ifndef NVAL 
#define NVAL 6 
#endif 


// GLOBAL variables 
sem_t sprod; 
sem_t scons; 
sem_t mx; 
int numOfConsumed; 
int topIndex; 
int arrayOfData[MAX_CAP]; 


void* produce (void* arg){ 
    int local_top; 
    sem_wait(&sprod); 
    sem_wait(&mx); 
    if (numOfConsumed >= NVAL){ 
        sem_post(&sprod); 
        sem_post(&mx); 
        pthread_exit((void*)0); 
    }

    local_top = topIndex + 1; 
    topIndex ++; 
    arrayOfData[local_top] = rand()%100 + 1; 
    printf("Produced: %d\n", arrayOfData[local_top]); 
    sem_post(&scons); 
    sem_post(&sprod); 
    sem_post(&mx); 

    pthread_exit((void*)0); 
} 

void* consume (void* arg){ 
    int local_top; 
    sem_wait(&scons); 
    sem_wait(&mx); 
    if (numOfConsumed >= NVAL ){ 
        sem_post(&scons); 
        sem_post(&mx); 
        pthread_exit((void*)0); 
    }

    local_top = topIndex; 
    topIndex --; 
    printf("Consumed data is: %d \n", arrayOfData[local_top]); 
    numOfConsumed ++; 
    sem_post(&mx); 

    pthread_exit((void*)0); 

}


// do i create threads for consumers and producers separately? 
// do i separate the for loop? 
// will it affect the order 
int main(int argc, char** argv){ 
    // tid for producers and consumers 
    pthread_t tidPROD[NUM_PROD]; 
    pthread_t tidCONS[NUM_CONS]; 

    // i: used to create producer threads 
    // j: used to create consumer threads 
    int i, j, p, c; 

    // join: for main to wait for all threads to finish 
    int statusP, statusC;

    // initialize global variables 
    sem_init(&sprod, 0, MAX_CAP); 
    sem_init(&scons, 0, 0); 
    sem_init(&mx, 0, 1); 
    numOfConsumed = 0; 
    topIndex = -1; 
    for (int k=0; k<MAX_CAP; k++){ 
        arrayOfData[k]=-1; 
    }


    for (i=0; i<NUM_PROD; i++) { 
        if (pthread_create((pthread_t*)&tidPROD[i], NULL, produce, NULL) != 0){ 
            perror("pthread_create\n"); 
            exit(1); 
        }
    }

    for (j=0; j<NUM_CONS; j++) { 
        if (pthread_create((pthread_t*)&tidCONS[i], NULL, consume, NULL) != 0){ 
            perror("pthread_create\n"); 
            exit(1); 
        }
    }

    for (p = 0; p < NUM_PROD; p++) {
        if(pthread_join(tidPROD[p], (void**)&statusP) != 0) {
            perror("join");
            exit(1);
        } else { 
            printf("PROCUDER Thread %d finished: returned %d \n", p+1, statusP); 
        }
    } 

    for (c = 0; c < NUM_CONS; c++) {
        if(pthread_join(tidCONS[c], (void**)&statusC) != 0) {
            perror("join");
            exit(1);
        } else { 
            printf("Consumer Thread %d finished: returned %d \n", c+1, statusC); 
        }
    } 

    printf("End of the program.\n"); 

    return EXIT_SUCCESS; 
}