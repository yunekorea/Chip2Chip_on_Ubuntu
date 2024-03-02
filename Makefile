CC = gcc
AR = ar
CXX = g++

all:
	$(CC) -o $@ $^

clean:
	rm *.o *.out
