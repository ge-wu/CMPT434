# Assignment 2

## Part A
Implementation a sliding window protocol to ensure reliable in-order transmission
of data over UDP socket using C programming language. Here is a 
[link](https://www2.tkn.tu-berlin.de/teaching/rn/animations/gbn_sr/) of a 
sliding window simulator that I found very helpful to understand how it works. 

### Documentation
The implementation is mainly by studying *Figure 3-21. A sliding window protocol
using selective repeat* from Tanenbaum's Computer Network. The source code is 
very exquisite and ingenious, it benefits learners a lot. Unfortunately, I don't 
have enough talent to reproduce it perfectly.  

For the `network.h`, UDP client and server generators are used 
directly from Assignment 1. I also add some structures to better organize data, 
the most important one is the `frame` structure. It contains important information that 
both the sender and receiver need to slide their window. 

Both the sender and the receiver have a same size window. There does not exist 
an 'actual' window, rather two integers to represent the lower and upper edges of 
the window. The window will only shift **forward** if an frame that has 
sequence number equal to each receiver or sender's lower edge.

Please check `PartA-Design.pdf` for more details. 

### Installing
You need to have `gcc` and run it on **Linux** machine, recommend to run on Tuxworld 8
. Change to the project 
director and run following commands: 
```
$ make
```
### Running
The program needs two terminals, one for the receiver and another one for the sender.
- Start the receiver
```
$ ./receiver <port> <window size>
```
example:
```
$ ./receiver 30000 2
```
- Start the sender
```
$ ./sender <IP> <port> <window size> <timeout>
```
example:
```
$ ./sender tux8 30000 2 10
```
There are plenty of ways to check IP address on Linux machine. Simply, `hostname
-I`. If this not working, please Google yourself for other ways. Alterntely, you can use 
`tux8` or `tux8.usask.ca` which are guarantee work if you run this on Tuxworld. 

*Note: I restrict the uer can only use port 30000. The windows size of the 
receiver and the sender should be the same. The unit of timeout is second.*

### Simulating Packet Loss
Once the receiver received a frame from the sender. It will prompt the user
for input. If a line of text beginning with "Y" is input, the receiver should 
consider the message to have been correctly received and a ACk frame will 
send back to the sender; otherwise, the receiver 
should consider the message to have been corrupted or dropped within the network
and nothing happen. 
