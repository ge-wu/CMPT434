# CMPT 434 Assignment 1 Part A
# Jiaye Wang jiw561 11231145

CC = gcc
CFLAGS = -g
CPPFLAGS = -Wall -pedantic 

EXE = udp_server tcp_server tcp_client proxy


.PHONY: clean all

all: $(EXE)

tcp_client: tcp_client.c network.o
udp_server: udp_server.c network.o cmd_processor.o
tcp_server: tcp_server.c network.o cmd_processor.o
proxy: proxy.c network.o cmd_processor.o

network.o: network.c network.h 
cmd_processor.o: cmd_processor.c cmd_processor.h

clean:
	$(RM) *.o $(EXE)