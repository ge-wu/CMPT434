# Assignment 3
## Part A
Implementation of or a Epidemic protocol (Gossip protocol) with C programming 
language. A sensor reprsents a node and the sensor will able to transmit 
data between each other.  Please check `PartA-Design.pdf` for more
implementation details. 

#### Installing
You need to have `gcc` and run it on Linux machine, recommend to run on Tuxworld
8. Change to the project directory and run the following command
```shell
make
```

#### Running
You need at least one and at most 26 terminals. One terminal runs one sensor. 
-  Start a seonsor
```
./sensor <ID> <port> <storage space> <msg>
```
example: 
```
./sensor A 30000 5 hello
```

#### Remark
- The application is default to run on Tuxworld 8. If the user hope to 
run on other host. Please search `tux8` inside `sensor.c` and replace 
it with your preference. 
- Each sensor's ID and port must be unqiue. 
- ID can only be an uppercase English alphabet, so that there are at most 26 
sensors are allowed. 
- `<msg>` must be a single word (without spaces) that at most 10 characters.  
## Part B
Please check `PartB.pdf`.
