#include <fstream>
#include "Client.h"


Client::~Client() {
    // Close the connection to the server and release resources
    if (close(this->client_socket) < 0) {
        perror("Error closing socket");
    }
}

Client::Client(const char *server_ip_address, const int server_port) {
    // Initialize the client socket
    this->port = server_port;
    this->ip_address = server_ip_address;
    this->flag = 0;
}

void Client::createSocket() {
    // Create a socket point
    this->client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->client_socket < 0) {
        perror("Error creating socket");
        exit(0);
    } else {
        cout << "socket created" << endl;
    }
}

void Client::connectToServer() {
    if (this->flag == 0) {
        // set up server address
        struct sockaddr_in server_address;
        memset(&server_address, 0, sizeof(server_address));
        server_address.sin_family = AF_INET;
        server_address.sin_addr.s_addr = inet_addr(this->ip_address);
        server_address.sin_port = htons(this->port);


        // connect to server
        if (connect(this->client_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
            perror("Error connecting to server");
            // end the program
            this->~Client();
            exit(0);
        } else {
            cout << "Connected to server" << endl;
        }
        this->flag = 1;
    }
}

void Client::getInput() {
    // get input from user
    string input;
    getline(cin, input);
    strcpy(this->query, input.c_str());
}

void Client::writeStr(string input){
    write(this->client_socket,input.c_str(),input.length());
    cout<< "command written" << endl;
}

// read a line from the socket
string Client::readStr() {
    // read string line
    string server_output;
    char c=0;
    read(this->client_socket,&c,sizeof(char));
    while(c!='\n'){
        server_output+=c;
        read(this->client_socket,&c,sizeof(char));
    }
    return server_output;
}

// read the menu from the server
void Client::readMenu() {
    // read string line
    bool done=false;
    while(!done){
        // read string line
        string serverInput = readStr();
        if(serverInput=="8. exit")
            done=true;
        cout << serverInput << endl;
    }
}
//
//
//int Client::sendToServer() {
//    // send data to server
//    if (send(this->client_socket, this->query, strlen(this->query), 0) < 0) {
//        perror("Error sending query to server, please try again");
//        return 0;
//    } else {
//       // cout << "Query sent to server" << endl;
//    }
//    return 1;
//}
//
//
//void Client::receiveFromServer() {
//    // receive data from server
//    char buffer[4096];
//    int read_bytes = recv(this->client_socket, buffer, sizeof(buffer), 0);
//    if (read_bytes < 0) {
//        perror("Error receiving label from server");
//    } else if (read_bytes == 0) {
//        cout << "connection is closed" << endl;
//        this->~Client();
//        exit(0);
//    } else {
//        cout << buffer << endl;
//    }
//}

// main to runServer client side
// only place we need to use thread is when getting a file to save (option 5 in the menu)
// there is no need to use thread for the other options in client side
// in order for the client to send commands in parallel to getting data from the server we need to use threads -
// one thread for sending commands and one thread for getting data from the server
// this way the client can always send more commands and is not waiting for the server to send data
// send data to the server that the user entered and prints the text that the server sent back
// when the client is connected to the server the server will send to the client text of the menu
// the client will print the menu and the user will choose an option

int main(int argc, char *argv[]) {
    // check for correct number of arguments
    if (argc != 3) {
        cout << "Invalid input" << endl;
        return 0;
    }
    // check for valid ip address
    vector<string> parts;
    istringstream ss(argv[1]);
    string part;
    while (getline(ss, part, '.')) {
        parts.push_back(part);
    }
    if (parts.size() != 4) {
        cout << "Invalid input" << endl;
        return 0;
    }
    for (const string &part: parts) {
        size_t pos;
        int num = stoi(part, &pos);
        if (pos != part.size() || num < 0 || num > 255) {
            cout << "Invalid input" << endl;
            return 0;
        }
    }
    // check for valid port number
    size_t pos;
    int port = stoi(argv[2], &pos);
    if (pos != string(argv[2]).size() || port < 0 || port > 65535) {
        cout << "Invalid input" << endl;
        return 0;
    }

    // create client object that will runServer indefinitely
    const char *ip_address = argv[1];
    Client client(ip_address, port);
    // connect to server
    client.createSocket();
    client.connectToServer();

    while (client.query != "8") {
        // get input from user
        client.readMenu();
        client.getInput();

        // if client input is -1 then exit
        if (strcmp(client.query, "8") == 0) {
            break;
        }
        // send data to server
        client.writeStr(strcat(client.query,"\n"));
        // if client choice is 1 then upload file and send to server as string

        if (client.query[0] == '1'){
            cout << client.readStr() << endl;
            char buffer[4096];
            string fileName;
            cin>> fileName;
//            getline(cin, fileName);
            ifstream in(fileName);
            string line ="";
            while (getline(in, line)) {
                cout<<line<<endl;
                client.writeStr(line);
            }
        }

        // if client chose 2 send the server the k and metric string
        if (client.query[0] == '2') {
            cout << client.readStr() << endl;
            char buffer[4096];
            // get input from user
            string input;
            getline(cin, input);
            strcpy(buffer, input.c_str());
            client.writeStr(buffer);
        }



//        // receive label from server
//        client.receiveFromServer();
    }
    client.~Client();
    exit(0);
}

