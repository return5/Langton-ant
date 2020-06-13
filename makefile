CFLAGS = -std=gnu99 -Wall -Werror -O2
LIBFLAGS = -lncurses
CC = gcc
OUTFILE = ./ant

all:
	$(CC) $(CFLAGS) -o $(OUTFILE) main.c $(LIBFLAGS)

clean:
	rm -f *.o *.gch
