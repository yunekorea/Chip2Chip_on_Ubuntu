main.o : main.c main.h
	gcc -c main.c

main.out : main.o
	gcc -o main.out main.o

clean :
	rm *.o
