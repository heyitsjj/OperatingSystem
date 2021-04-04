/**** decoder.c ****/

#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


#define _XOPEN_SOURCE 700


int magicsq[3][3] = {{4, -1, 8}, {-1, -1, -1}, {2, -1, 6}};

int row[5] = {0, 1, 1, 1, 2};
int col[5] = {1, 0, 1, 2, 1};


void display() {
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++)
            printf("  %d", magicsq[i][j]);
        printf("\n");
    }
    printf("\n");
}

int check() {

	int i, j, sums[8];
	for (i = 0; i < 8; i++) 
		sums[i] = 0;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			sums[i] += magicsq[i][j];
			sums[i+3] += magicsq[j][i];
			if (i == j) sums[6] += magicsq[i][j];
			if ((i+j) == 2) sums[7] += magicsq[i][j];
		}
	}
	int result = 1;
	i = 1;
	while ((i < 8) && (result == 1)) {
		if (sums[0] != sums[i]) {
            result = 0;
		}
		i++;
	}
	return result;
}





int solve(int x) {
	/* CODE THAT EVERY CHILD PROCESS EXECUTES */
	magicsq[0][1] = x;
	for (int b = 0; b <= 9; b++) {
	    for (int c = 0; c <= 9; c++) {
	        for (int d = 0; d <= 9; d++) {
	            for (int e = 0; e <= 9; e++) {
	                magicsq[row[1]][col[1]] = b;
                    magicsq[row[2]][col[2]] = c;
                    magicsq[row[3]][col[3]] = d;
                    magicsq[row[4]][col[4]] = e;
                    if (check()) {
                        return 1;
                    }
	            }
	        }
	    }
	}
	return 0;
}

int main(int argc, char **argv)
{	
	for (int i = 0; i < 10; i++) {
	    if (fork() == 0) {
	        if (solve(i)) {
	            display();
	        }
	        exit(0);
	    }
	}
    while (wait(NULL) > 0) {
        // do noting
    }
    return(0);
}
