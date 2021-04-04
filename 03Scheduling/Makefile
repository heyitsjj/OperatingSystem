CFLAGS=-Iinclude -Wall
FILE=tasks
POLICY1=FCFS
POLICY3=SRTF
POLICY4=RR
POLICY5=MFQ
POLICY6=IORR
#POLICY values
#1	FCFS
#2	SJF
#3	SRTF
#4	RR 4
#5	MFQ 3
#6	IORR 4

all: build run

build: clean
	mkdir obj
	gcc $(CFLAGS) -c -o obj/policies.o src/sched-policies.c	
	gcc $(CFLAGS) -c -o obj/simulator.o src/sched-simulator.c
	mkdir bin
	gcc $(CFLAGS) -o bin/sched-simulator obj/simulator.o obj/policies.o

run:
	bin/sched-simulator $(FILE) $(POLICY3) 1>sched-run-SRTF.txt
	bin/sched-simulator $(FILE) $(POLICY4) 4 1>sched-run-RR.txt
	bin/sched-simulator $(FILE) $(POLICY5) 3 1>sched-run-MFQ.txt
	#bin/sched-simulator $(FILE) $(POLICY6) 4 
	cat *.txt

clean:
	rm -rf bin obj sched-run-SRTF.txt sched-run-RR.txt sched-run-MFQ.txt
