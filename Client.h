#ifndef ASS4AP_CLIENT_H
#define ASS4AP_CLIENT_H

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class Client {
private:
    int port;
    const char *ip_address;
    int client_socket;
    int flag;


public:
    Client(const char *server_ip_address, int server_port);

    void connectToServer();

    void createSocket();


    void getInput();

    ~Client(


    );

    char query[4096];

    string readStr();

    void readMenu();

    void writeStr(string input);
};

#endif //ASS4AP_CLIENT_H
