// Jiaye Wang jiw561 11231145
#ifndef _TCP_H
#define _TCP_H

#define BACKLOG 10

/* 
 * Initialize a TCP server socket. 
 * Args:
 *  host: hostname of the running machine.
 *  port: A port value between 30000 to 40000.
 * Return:
 *  Positive integer for TCP server socket. 
 *  Return negative for failed initialization. 
 */
int tcp_server_init(const char *host, const char *port);

// Initialize a TCP client socket. 
int tcp_client_init(const char *port);

// 
int tcp_accept(int sockfd);


#endif

