#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h> 
#include <semaphore.h>


#ifndef NUM_THREADS  
#define NUM_THREADS 2 
#endif 

// GLOBAL variables 
int numOfThreadsLeft; 
sem_t mx; 


void* convertToUpper (void* arg) {
    sem_wait(&mx); 
    if (numOfThreadsLeft <= 0){ // when there are more files to convert than the number of threads, kill threads 
        sem_post(&mx); 
        pthread_exit((void*)0); 
    } else{ 
        numOfThreadsLeft --; 
    }
    sem_post(&mx); 

    FILE *fp1, *fp2;
    char dest_fname[128];
    int c = 1;
    
    fp1 = fopen (arg, "r");
    strcpy(dest_fname, arg);
    strcat(dest_fname, ".UPPER.txt");
    printf("%s\n", dest_fname);
    fp2 = fopen (dest_fname, "w");
    if ((fp1 == NULL) || (fp2 == NULL)) {
        perror ( "fopen");
        exit (1);
    }
    
    while (c != EOF) {
        c = fgetc(fp1);
        if (c != EOF)
            fputc(toupper(c), fp2);
//            fputc(toupper(c), stdout);
    }
    
    fclose (fp1);
    fclose (fp2);
    
    free(arg); 

    pthread_exit((void*)0);
}


//how would i know how many files need to be converted ?? 
// how many files in the argument ?? 
int main (int argc, char** argv){ 
    pthread_t tid[NUM_THREADS]; 
    int i, j, status; 
    char* pt_char; 

    //initialize global variables 
    sem_init(&mx, 0, 1); 
    numOfThreadsLeft = NUM_THREADS; 

    for (i = 0; i < (argc-1); i++) { 
        pt_char = (char*)malloc(sizeof(char)*30);
        strcpy(pt_char, argv[i+1]); 
        if (pthread_create((pthread_t*)&tid[i], NULL, convertToUpper, (void*)pt_char) != 0){ 
            perror("pthread_create\n"); 
            exit(1); 
        }
    }

    for (j = 0; j < (argc-1); j++) {
        if(pthread_join(tid[j], (void**)&status) != 0) {
            perror("join");
            exit(1);
        } 
    } 

    printf("End of the program.\n"); 

    return EXIT_SUCCESS; 
}
