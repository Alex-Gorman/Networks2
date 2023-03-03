CC = gcc
CFLAGS = -g
CPPFLAGS = -Wall

all: receiver sender

receiver: receiver.o
	$(CC) -o receiver receiver.o

receiver.o: receiver.c
	$(CC) -o receiver.o -c $(CFLAGS) $(CPPFLAGS) receiver.c

sender: sender.o
	$(CC) -o sender sender.o

sender.o: sender.c
	$(CC) -o sender.o -c $(CFLAGS) $(CPPFLAGS) sender.c

clean:
	@echo "Cleaning up..."
	@rm -f *.o
	@rm -f receiver
	@rm -f sender
