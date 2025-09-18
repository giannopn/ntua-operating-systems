#
# Makefile
# 

CC = gcc

# CAUTION: Always use '-pthread' when compiling POSIX threads-based
# applications, instead of linking with "-lpthread" directly.
CFLAGS = -Wall -O2 -pthread
LIBS = 

all: simplesync-mutex simplesync-atomic mandel mandel-semaphores mandel-condition-var

## Simple sync (two versions)
simplesync-mutex: simplesync-mutex.o
	$(CC) $(CFLAGS) -o simplesync-mutex simplesync-mutex.o $(LIBS)

simplesync-atomic: simplesync-atomic.o
	$(CC) $(CFLAGS) -o simplesync-atomic simplesync-atomic.o $(LIBS)

simplesync-mutex.o: simplesync.c
	$(CC) $(CFLAGS) -DSYNC_MUTEX -c -o simplesync-mutex.o simplesync.c

simplesync-atomic.o: simplesync.c
	$(CC) $(CFLAGS) -DSYNC_ATOMIC -c -o simplesync-atomic.o simplesync.c

## Mandel
mandel: mandel-lib.o mandel.o
	$(CC) $(CFLAGS) -o mandel mandel-lib.o mandel.o $(LIBS)

mandel-lib.o: mandel-lib.h mandel-lib.c
	$(CC) $(CFLAGS) -c -o mandel-lib.o mandel-lib.c $(LIBS)

mandel.o: mandel.c
	$(CC) $(CFLAGS) -c -o mandel.o mandel.c $(LIBS)

## Mandel Semaphores
mandel-semaphores: mandel-lib.o mandel-semaphores.o
	$(CC) $(CFLAGS) -o mandel-semaphores mandel-lib.o mandel-semaphores.o $(LIBS)

mandel-semaphores.o: mandel-semaphores.c
	$(CC) $(CFLAGS) -c -o mandel-semaphores.o mandel-semaphores.c $(LIBS)

## Mandel Condition Variable
mandel-condition-var: mandel-lib.o mandel_condition_var.o
	$(CC) $(CFLAGS) -o mandel-condition-var mandel-lib.o mandel_condition_var.o $(LIBS)

mandel_condition_var.o: mandel-condition-var.c
	$(CC) $(CFLAGS) -c -o mandel_condition_var.o mandel-condition-var.c $(LIBS)

clean:
	rm -f *.s *.o simplesync-mutex simplesync-atomic mandel mandel-semaphores mandel-condition-var
