CC = gcc
AR = ar
CXX = g++

all:main

main: main.h hils_op.o result_process.o tag_mgmt.o trace_parser.o
	cc $(CFLAGS) -g -o main main.o

hils_op.o: hils_op.h types.h
	gcc $(CFLAGS) -c -g hils_op.c

result_process.o: result_process.h types.h
	gcc $(CFLAGS) -c -g result_process.c

tag_mgmt.o: tag_mgmt.h types.h
	gcc $(CFLAGS) -c -g tag_mgmt.c

trace_parser.o: trace_parser.h types.h
	gcc $(CFLAGS) -c -g trace_parser.c

clean:
	rm *.o *.out
