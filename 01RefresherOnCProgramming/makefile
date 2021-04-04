CC=gcc -Wall
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src


all: runstack runfifo 

#Q2: archive the stack_array.o in libstack.a 
${LIB}/libstack.a: ${OBJ}/stack_array.o 
	ar -crs $@ $^ 

#Q3: gcc -D STACK_SIZE
${OBJ}/stack_array.o: ${INC}/stack.h ${SRC}/stack_array.c 
	${CC} -c -o ${OBJ}/stack_array.o ${SRC}/stack_array.c -I${INC} -DSTACK_SIZE=13
${OBJ}/stack_test.o: ${INC}/stack.h ${SRC}/stack_test.c 
	${CC} -c -o ${OBJ}/stack_test.o ${SRC}/stack_test.c -I${INC} -DSTACK_SIZE=13 

#Q4: Complete list_impl.c 
${OBJ}/list_impl.o: ${INC}/list.h ${SRC}/list_impl.c 
	${CC} -c -o ${OBJ}/list_impl.o ${SRC}/list_impl.c -I${INC} 

#Q5: implement stack.h in stack_list.c using primitives from list.h and build a new library libstack.a 
${OBJ}/stack_list.o: ${INC}/stack.h ${INC}/list.h ${SRC}/stack_list.c  
	${CC} -c -o ${OBJ}/stack_list.o ${SRC}/stack_list.c -I${INC} 
${LIB}/libstack.a: ${OBJ}/stack_list.o ${OBJ}/list_impl.o 
	ar -rsv $@ $^

#binary stack_test 
${BIN}/stack_test: ${OBJ}/stack_test.o ${LIB}/libstack.a 
	${CC} -o ${BIN}/stack_test ${OBJ}/stack_test.o ${LIB}/libstack.a 



#Q6: implement fifo.h in fifo_list useing the primitives from list.h 
#    build a new library libfifo.a from fifo_list.c and list_impl.c 
#    test program fifo_test.c  
${OBJ}/fifo_list.o: ${INC}/fifo.h ${INC}/list.h ${SRC}/fifo_list.c 
	${CC} -c -o ${OBJ}/fifo_list.o ${SRC}/fifo_list.c -I${INC} 

${LIB}/libfifo.a: ${OBJ}/fifo_list.o ${OBJ}/list_impl.o
	ar -crs $@ $^ 

${OBJ}/fifo_test.o: ${INC}/fifo.h ${SRC}/fifo_test.c 
	${CC} -c -o ${OBJ}/fifo_test.o ${SRC}/fifo_test.c -I${INC} 

${BIN}/fifo_test: ${OBJ}/fifo_test.o ${LIB}/libfifo.a
	${CC} -o ${BIN}/fifo_test ${OBJ}/fifo_test.o ${LIB}/libfifo.a 



runstack: ${BIN}/stack_test
	echo "Testing Stack"
	${BIN}/stack_test

runfifo: ${BIN}/fifo_test
	echo "Testing fifo"
	${BIN}/fifo_test

clean:
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*
