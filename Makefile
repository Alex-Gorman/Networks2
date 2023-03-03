CC = gcc
CFLAGS = -g
CPPFLAGS = -Wall

all: receiver sender

list_adders.o: list_adders.c list.h
	$(CC) -o list_adders.o -c $(CFLAGS) $(CPPFLAGS) list_adders.c


receiver: receiver.o
	$(CC) -o receiver receiver.o

receiver.o: receiver.c
	$(CC) -o receiver.o -c $(CFLAGS) $(CPPFLAGS) receiver.c

sender: sender.o
	$(CC) -o sender sender.o list_adders.o

sender.o: sender.c list.h list_adders.o
	$(CC) -o sender.o -c $(CFLAGS) $(CPPFLAGS) sender.c

clean:
	@echo "Cleaning up..."
	@rm -f *.o
	@rm -f receiver
	@rm -f sender