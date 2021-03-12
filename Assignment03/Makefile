# Jiaye Wang jiw561 11231145

CC = gcc
CFLAGS = -g
CPPFLAGS = -Wall -pedantic 

EXE = sensor


.PHONY: clean all

all: $(EXE)

sensor: sensor.c tcp.o

tcp.o: tcp.c tcp.h

clean:
	$(RM) *.o $(EXE)
