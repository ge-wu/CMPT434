<!-- Jiaye Wang jiw561 11231145 -->
# Assignment 2

## Part A
Implementation a sliding window protocol to ensure reliable in-order transmission
of data over UDP socket using C programming language. Here is a 
[link](https://www2.tkn.tu-berlin.de/teaching/rn/animations/gbn_sr/) of a 
sliding window simulator that I found very helpful to understand how it works. 

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
-I`. If this not working, please Google yourself for other ways. Alternately, you can use
`tux8` or `tux8.usask.ca` which are guarantee work if you run this on Tuxworld.


### Remarks
- **Window size of the receiver and sender must be the same**
- The sender and receiver can only use port 30000
- The window size is limited from 1 to 7, inclusive. The 
timeout must be at least 1 second and at most 30 seconds. 
- Whenever the sender or receiver is asking for an input. Input immediately, 
especially for the receiver. The user must input something that does not start with
"Y" to simulate the packet lost or not received. 
- Even though the program is using UDP transmission, still recommend to restart 
both the sender and receiver at the same time. Although, the order to start 
them does not matter. 
- The program does not check the length of the user input, because that is not 
important for the purpose of this assignment. 


## Part B
Please check `PartB.pdf`.
