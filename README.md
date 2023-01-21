# AP-PART3
Advanced Programming Assignment #3
<br> by Yair and Sigal and written in c++.
This program implements KNN classification algorithm based on a Client-Server architecture.

# Files
Files are located in the following directories:
* Client:
* `client.cpp` - client side of the program
* `client.h` - client header file

* Server:
* `server.cpp` - server side of the program
* `server.h` - server header file
* `knn.cpp` - knn algorithm implementation
* `knn.h`  - class implements Knn algorithm - classifies new query using classified data.
  Uses five different distance methods in order to set the class for the input:
    * euclideanDistances AUC <br>
    * manhattanDistance as MAN<br>
    * ChevishevDistance as CHB<br>
    * canberraDistance as CAN<br>
    * minkowskiDistance - P in the equation is a constant (set to be 2.0) given and not chosen by the user. MIN<br>
* `CheckValid.cpp` - checking if the input is valid
* `CheckValid.h` - checking if the input is valid header file
* `Dataset.cpp` - dataset class implementation
* `Dataset.h` - dataset class header file
* `ReadData.cpp` - reading the data from the file
* `ReadData.h` - reading the data from the file header file


* `Makefile` - makefile for the program
* `README.md` - this file

# installation (running on UNIX)
To runServer the program you should do the following:<br>
1. Download the zip of the code from git.
2. Use a c++ compiler (download and install if needed).
3. Open the terminal and runServer the following commends:<br>
    1. * `make` - to compile the program.
    * `.out` - to runServer the program. (note section ii)
    * `make clean` - to clean the directory from the compiled files.<br><br>

    2. * `server.out` file port - to runServer the server write the command like this:<br>
         ./server.out <your file name> <port> (use space between the file name and the port number)
    * `client.out` vector Distance K - to runServer the client write the command like this:<br>
      ./server.out <distance> <K> (use space between each argument)<br>

# USAGE:
1. The client will be asked to enter a query with instructions.
    * `example:`
      6.9 3.1 4.9 1.5 MAN 4
2. Once the client entered the query, the server will calculate the distance between the query and the data and return the answer to the client.
    * `Running example:`
      <br>1 2 3 4 MAN 3 <br>
      <br>Iris-versicolor<br>

# NOTES:
1. The server is running on a loop, so you can runServer the client as many times as you want.
2. The client is running on a loop, so you can enter as many queries as you want. To end the client, enter "-1" as the query.
3. When choosing K for KNN it should be a positive integer and less than the classified data size.
4. For invalid classified data - the program will return "invalid input" and you will get to input a new query.
