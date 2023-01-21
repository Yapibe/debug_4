//
// Created by User on 29/12/2022.
//

#ifndef ASS4AP_SERVER_H
#define ASS4AP_SERVER_H


#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <vector>
#include "ReadData.cpp"
#include <stdexcept>
#include <thread>
#include "Dataset.h"
#include "Knn.h"
#include "CheckValid.h"
#include "CLI.h"


using namespace std;

class ClientHandler {
public:
    virtual void handle(int clientID) = 0;
};

class KnnHandler : public ClientHandler {
public:
    virtual void handle(int clientID) {
        // create a socketIO object and a CLI object
        auto *dio = new SocketIO(clientID);
        CLI *cli = new CLI(dio);
        // start CLI
        cli->start();
    }
};

class Server {
protected:
    thread *t; // the thread to runServer the start() method in
    int fd;
    sockaddr_in server;
    sockaddr_in client;
    volatile bool run = true;

public:
    Server(int port) throw(const char *);
    virtual ~Server();
    void start(ClientHandler &ch) throw(const char *);
    void stop();

    basic_string<char> getInfoFromClient();
    void sendToClient(string result);

    void parseStringFromUser(string str, string &vector, string &k, string &function);


    void runServer(vector<vector<double>> data_vectors, vector<string> labels);

    void ConnectToClient();
};

#endif //ASS4AP_SERVER_H
