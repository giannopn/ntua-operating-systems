CC = gcc
CFLAGS = -Wall

all: main-source-code ex01 ex02-1 ex02-2 ex02-3 ex02-4 ex03

main-source-code: main-source-code.c
	$(CC) $(CFLAGS) $< -o $@

ex01: ex01.c
	$(CC) $(CFLAGS) $< -o $@

ex02-1: ex02-1.c
	$(CC) $(CFLAGS) $< -o $@

ex02-2: ex02-2.c
	$(CC) $(CFLAGS) $< -o $@

ex02-3: ex02-3.c main-source-code
	$(CC) $(CFLAGS) ex02-3.c -o ex02-3

ex02-4: ex02-4.c
	$(CC) $(CFLAGS) $< -o $@

ex03: ex03.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f main-source-code ex01 ex02-1 ex02-2 ex02-3 ex02-4 ex03
