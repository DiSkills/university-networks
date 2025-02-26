SRCMODULES = server.c session.c student.c
OBJMODULES = $(SRCMODULES:.c=.o)

CC = gcc
CFLAGS = -ggdb -Wall

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

prog: main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@

ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif

deps.mk: $(SRCMODULES)
	$(CC) -MM $^ > $@

clean:
	rm prog *.o deps.mk
