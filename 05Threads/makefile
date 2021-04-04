CFLAGS=-Iinclude -Wall
LIBRARIES=-lrt -lpthread

## SETTINGS ##
# Change the integer values below to set:

# 	Q2: the nb of threads allowed to create in Q2 
NB_THREADS=-DNUM_THREADS=2
# 	Q3: the nb of producers, consumers in Q3 
#       the value of MAX_CAP and NVAL in Q3 
NB_PROD=-DNUM_PROD=5
NB_CONS=-DNUM_CONS=5
VAL_MAX_CAP=-DMAX_CAP=10
VAL_NVAL=-DNVAL=6

#########################


all: build runmulti1 runmulti2 runbounded 

build: clean
	mkdir obj
	gcc $(CFLAGS) -c -o obj/multi-upper1.o q1multi-upper.c 
	gcc $(CFLAGS) -c -o obj/multi-upper2.o q2multi-upper.c $(NB_THREADS)
	gcc $(CFLAGS) -c -o obj/bounded-buffer.o q3bounded-buffer.c $(NB_PROD) $(NB_CONS) $(VAL_MAX_CAP) $(VAL_NVAL)
	mkdir bin
	gcc $(CFLAGS) -o bin/multi1 obj/multi-upper1.o $(LIBRARIES)
	gcc $(CFLAGS) -o bin/multi2 obj/multi-upper2.o $(LIBRARIES)
	gcc $(CFLAGS) -o bin/bounded obj/bounded-buffer.o $(LIBRARIES)

runmulti1: 
	bin/multi1 file1

runmulti2: 
	bin/multi2 file1 file2 file3 

runbounded: 
	bin/bounded 

clean:
	rm -rf bin obj file1.UPPER.txt file2.UPPER.txt file3.UPPER.txt 

