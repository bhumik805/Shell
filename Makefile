# Makefile
CC = gcc
CFLAGS = -Wall -g
OBJS = main.o tokenizer.o signal_handler.o

shell: $(OBJS)
	$(CC) $(CFLAGS) -o shell $(OBJS)

main.o: main.c shell.h
	$(CC) $(CFLAGS) -c main.c

tokenizer.o: tokenizer.c shell.h
	$(CC) $(CFLAGS) -c tokenizer.c

signal_handler.o: signal_handler.c shell.h
	$(CC) $(CFLAGS) -c signal_handler.c

clean:
	rm -f *.o shell