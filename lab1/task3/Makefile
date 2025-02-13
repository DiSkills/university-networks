SRCMODULES = student.c
OBJMODULES = $(SRCMODULES:.c=.o)

CC = gcc
CFLAGS = -ggdb -Wall

all: client server requests responses

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

client: client.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@

server: server.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@

-include deps.mk

deps.mk: $(SRCMODULES)
	$(CC) -MM $^ > $@

requests responses:
	mkfifo $@

clean:
	rm -f client server *.o deps.mk requests responses
