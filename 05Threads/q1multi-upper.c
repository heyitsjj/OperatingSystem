#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h> 



void* convertToUpper (void* arg) {
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


int main (int argc, char** argv){ 
    pthread_t tid[argc-1]; 
    int i, j, status; 
    char* pt_char; 

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
        //else { 
            //printf("%d Thread finished with converting: returned %d \n", j, status); 
        //}
    } 

    printf("End of the program.\n"); 

    return EXIT_SUCCESS; 
}
