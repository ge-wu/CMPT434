# Assignment 2

## Part A
I modified a linked-list data structure to a queue that my team mate and I implemented in CMPT 332. 
Here is the [link](https://git.cs.usask.ca/dwm138/group11/-/tree/a1-phase3/a1-phase3) of the repository. 
This purpose is to use the queue as the buffer of the protocol to make life
easier, such as, the size of the queue is equal to the window size, pop the
the first element if it got acknowledge and push back again if it did not get,
etc.
`q_test.c` includes some simple test cases for the queue. Use `make q_test` and 
`./q_test` to check the test output. These tests if for my own purpose, 
feel free to ignore :). 

There are plent of way to check IP address on Linux machine. Simply, `hostname -I`, 
please Google yourself for more other ways. Alterntely, you can use 
`tux8` or `tux8.usask.ca` which are guarantee work. 

