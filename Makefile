CC = gcc
CFLAGS = -c -Wall -Werror

xmalloc.o: xmalloc.c xmalloc.h
	$(CC) $(CFLAGS) xmalloc.c

test-%.o: test-%.c xmalloc.o
	$(CC) $(CFLAGS) test-$*.c

test-%: test-%.o xmalloc.h
	$(CC) -o prog-$* test-$*.o xmalloc.o

all: clean xmalloc.o test-1 test-2 test-3 test-4 test-5 test-6 test-7 test-1.o test-2.o test-3.o test-4.o test-5.o test-6.o test-7.o

run-%: clean xmalloc.o test-%.o test-%
	./prog-$*

clean:
	$(RM) *.o prog-*
