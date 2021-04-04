
First Name: Jingjing 

Last Name: Liu

NYU ID: jl9358

=======================================================================================
Files submitted: 

src:
	fifo_list.c: Question 6
	fifo_test.c: Question 6
	list_impl.c: Question 4, Question 5, Question 6
	stack_array.c: Question 1
	stack_list.c: Question 5
	stack_test.c: Question 1, Question2, Question 3, Question 5

include:
	fifo.h: Question 6
	list.h: Question 4, Question 5, Question 6
	stack.h: Question 1, Question 2, Question 3 and Question 5


=======================================================================================
Compilation rules in the Makefile: 
# make all: (Q2) generates and registers the array-based stack library (lib/libstack.a) 
            (Q3) compiles the stack with a new value for STACK_SIZE as a compilation directive and runs the stack test 
			(Q5) compiles the stack, implemented as a double-linked list, then integrates it in an update of libstack.a 
			(Q5) uses newlibstack to recompile the binary executable (bin/stack_test), then runs it 
			(Q6) compiles a dynamic queue implemented as a double-linked list, then integrates it in a library (lib/libfifo.a) 


=======================================================================================
Comments: 
The program ran successfully. However, there is a warning about "overriding commands for target 'lib/stack.a', 
because I first archived the stack_array.o in the library libstack.a and later on added stack_list.o and 
list_impl.o into the library libstack.a. 


=======================================================================================
Textual answers: 

Question 7:
Traversing the entire list to determine the number of items is too expensive. Therefore, add a counter in 
stack_list.c and a counter in fifo_list.c. Initialize the counter as equal to 0, and then increment it as 
element is inserted and decrement it as element is poped or extracted from the list. 
	