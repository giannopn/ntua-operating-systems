#
# Makefile
#

CC = gcc

# CAUTION: Always use '-pthread' when compiling POSIX threads-based
# applications, instead of linking with "-lpthread" directly.
CFLAGS = -Wall -O2 -pthread
LIBS =

all: ex03

ex03: ex03.o
	$(CC) $(CFLAGS) -o ex03 ex03.o $(LIBS)

ex03.o: ex03.c
	$(CC) $(CFLAGS) -c -o ex03.o ex03.c $(LIBS)

clean:
	rm -f *.s *.o ex03
