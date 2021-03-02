# Jiaye Wang jiw561 11231145

CC = gcc
CFLAGS = -g
CPPFLAGS = -Wall -pedantic 

EXE = receiver sender

.PHONY: clean all

all: $(EXE)

receiver: receiver.c network.o 
sender: sender.c network.o 

network.o: network.c network.h 

clean:
	$(RM) *.o $(EXE)
