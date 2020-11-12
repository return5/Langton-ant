CFLAGS = -std=gnu99 -Wall -Wextra -O2 
LIBFLAGS = -lncurses -I
CC = gcc
SRC = ./src/
HEADERS = headers
OUTFILE = ant

all:
	$(CC) $(CFLAGS) -o$(OUTFILE) $(SRC)*.c $(LIBFLAGS)$(HEADERS) 

clean:
	rm -f *.o *.gch
