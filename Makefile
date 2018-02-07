DEBUG = -g
CFLAGS = ${DEBUG} -Wall

EXEC1 = simple_test
OBJS1 = simple_test.o

EXEC2 = random_test
OBJS2 = random_test.o

all: ${EXEC1} ${EXEC2}

${EXEC1}: ${OBJS1} 
	g++ ${CFLAGS} -o ${EXEC1} ${OBJS1}

${OBJS1}: %.o: %.cpp my_malloc.h
	g++ ${CFLAGS} -c -o $@ $<

${EXEC2}: ${OBJS2}
	g++ ${CFLAGS} -o ${EXEC2} ${OBJS2}

${OBJS2}: %.o: %.cpp my_malloc.h
	g++ ${CFLAGS} -c -o $@ $<

clean:
	rm -f ${EXEC1} ${OBJS1} ${EXEC2} ${OBJS2}
