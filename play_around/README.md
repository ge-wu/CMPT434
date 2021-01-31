# Assignment 1

## Part 1
Simple implementation of TCP server/client and UDP server applications on 
Linux machine. The main goal is to practice socket programming in C. 
The information send back from the server has no actual meaning. 

The code is mainly referenced from chapter 6 of 
[Beej's Guide to Network Programming](https://beej.us/guide/bgnet/pdf/bgnet_usl_c_1.pdf)
and the UDP example from `man getaddrinfo(3)`. 

In addition to the UDP and TCP server, the application also include a proxy 
server. Note that this application does not include a UDP client. The goal
of the proxy server is to act as an intermediary between the client and the server, 
with some additional functions. The proxy server will always use TCP communicate 
with the client and TCP or UDP with the server depend on the port. 

Please check the design documentation for more detail and testing. 


### Usage
Change the directory to the program directory and run 
`make`. Four executable file should be generated: `tcp_server, udp_server, `
`proxy_server,` and `tcp_client`. 

#### Question 1
We need two terminals for this part. Run ```./tcp_server``` to start the TCP 
server in one terminal. Then run ```./tcp_client $(hostname) 30002``` in the 
other terminal. __The server program must run first.__ Now there should be 
an instruction of how to use the program and a promote user for inputs. 
The `all` command will not work without the proxy server. Hence, the actual 
server will treat as invalid command. Use the `quit` command to terminate 
the application is recommended. 

#### Question 2 & 3
We need three terminals for question 2 and question 3. One for UDP or TCP server, 
one for the proxy server and one for the client. 
First run `./tcp_server` or `./udp_server` in one terminal. Then run 
`./proxy_server $(hostname) PORT` in the second terminal to connect to the actual
server, where `PORT=30002` for 
the TCP server and `PORT=30003` for the UDP server. Finally run 
`./tcp_client $(hostname) 30001` in the last terminal to connect to the proxy
server. __It must follow this order__. Now we can use the `all` command with the 
proxy server.  It simply send seven requests to the actual server, concatenation 
seven responses with a header, and send back to the client. 

### Remarks
- The command must be single one word, without space. i.e. `Mon`, `Tue`, etc,...
`Mon Tue` works, but will not work as expected.
- Don't use client to connect to the TCP server if the proxy server 
is already connecting to the TCP server.  
- Don't connect the UDP server with TCP client. 

## Part 2
Please check `cmpt434_asgmt_1_part2.pdf`. 