# CMPT 434 Assignment 1 Part A
# Jiaye Wang jiw561 11231145

CC = gcc
CFLAGS = -g
CPPFLAGS = -Wall -pedantic 

# EXE = udp_server tcp_client tcp_server proxy_server
EXE = udp_server tcp_server tcp_client proxy


.PHONY: clean all

all: $(EXE)

udp_server: udp_server.c network.o
tcp_client: tcp_client.c network.o
tcp_server: tcp_server.c network.o
proxy: proxy.c network.o

network.o: network.c network.h 

clean:
	$(RM) *.o $(EXE)