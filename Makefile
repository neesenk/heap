CFLAGS += -O0 -g -Wall
all: test test2

test: test.o heap.h heap.o
	$(CC) $(CFLAGS) test.o heap.o -o test

test2: test2.o fibheap.h fibheap.o
	$(CC) $(CFLAGS) test2.o fibheap.o -o test2

clean:
	rm -rf *.o test2 test
