CC = gcc
CFLAGS = -Wall -O2 -m32 -O0

PROGRAMS = ex1 ex2 ex3 ex4 ex5 ex6 challenge

all: $(PROGRAMS)

ex1: ex1.c
	$(CC) $(CFLAGS) -o $@ $<

ex2: ex2.c
	$(CC) $(CFLAGS) -o $@ $<

ex3: ex3.c
	$(CC) $(CFLAGS) -o $@ $<

ex4: ex4.c
	$(CC) $(CFLAGS) -o $@ $<

ex5: ex5.c
	$(CC) $(CFLAGS) -o $@ $<

ex6: ex6.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(PROGRAMS)
