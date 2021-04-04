#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>


int main (int arg, char * argv []) {
    int i, j, p;
    for (i = 0; i <3; i ++) {
        if ((p = fork()) == 0) {
            printf( "i =% d \n", i);
            j = 0;
            while (j < i && ((p = fork()) == 0)) {
                j++;
            }
            if (p == 0) {
                printf ( "j =% d \n", j);
            } else {
                // waiting for grand children
                while (wait(NULL) > 0) {
                    // do noting
                }
            }
            exit(1);
        }
    }
    // waiting for children
    while (wait(NULL) > 0) {
        // do noting
    }
    // printf("All processes ended\n");
    return (0);
}

