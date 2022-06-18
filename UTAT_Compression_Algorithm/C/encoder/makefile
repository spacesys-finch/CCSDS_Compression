# need to link math library `-lm`
main.out: main.o encoder.o
	gcc main.o encoder.o -lm -o main.out

# debug symbols turned on `-g`, has to be before `-c` compile
main.o: main.c 
	gcc -g -c main.c -o main.o

encoder.o: encoder.c encoder.h
	gcc -g -c encoder.c -o encoder.o

test.o: test.c
	gcc -g -c test.c -o test.o

test.out: test.o encoder.o
	gcc test.o encoder.o -lm -o test.out

clean:
	rm *.o
	rm *.out

run: main.out
	./main.out

debug: main.out
	gdb main.out

test: test.out
	./test.out