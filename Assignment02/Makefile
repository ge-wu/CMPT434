# Jiaye Wang jiw561 11231145

CC = gcc
CFLAGS = -g
CPPFLAGS = -Wall -pedantic 

EXE = receiver sender


.PHONY: clean all

all: $(EXE)

receiver: receiver.c network.o queue.o
sender: sender.c network.o queue.o

q_test: q_test.c queue.o

network.o: network.c network.h 
queue.o: queue.c queue.h
protocol.o: protocol.c protocol.h

clean:
	$(RM) *.o $(EXE)
