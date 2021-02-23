# Assignment 2

## Part A
I modified a linked-list data structure to a queue that my team mate and I implemented in CMPT 332. 
Here is the [link](https://git.cs.usask.ca/dwm138/group11/-/tree/a1-phase3/a1-phase3) of the repository. 
The `front()` and `pop()` functions follow C++ style, that is, `front()` will 
return return the first element and `pop()` will just remove the first element. 
This purpose is to create the queue data structure is to use it as the buffer of
the protocol, such as, the size of the queue is equal to the window size, pop the
the first element if it got acknowledge and push back again if it did not get,
etc.
`q_test.c` includes some simple test cases for the queue. Run `make q_test` and 
`./q_test` to check the test output. These tests are for my own purpose, 
feel free to ignore :). 

Timeout is use [this](https://www.gnu.org/software/libc/manual/html_node/Server-Example.html) 
as references to setup. 

There are plent of way to check IP address on Linux machine. Simply, `hostname
-I`. If this not working, please Google yourself for other ways. Alterntely, you can use 
`tux8` or `tux8.usask.ca` which are guarantee work if you run this on Tuxworld. 

Here is a quick demo, note that I use `(R)` denote receiver and `(S)` for
sender. The middle dash line represent the seperator of the two terminals. 

Sender sends new frame is always the most primary action to the user. That is, 
the user cannot processing receiver if the sender side is promoting the user 
for input. 

