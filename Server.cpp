
#include "Server.h"

// constructor

Server::Server(int port) throw(const char *) {
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        throw "socket failed";
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    if (bind(fd, (struct sockaddr *) &server, sizeof(server)) < 0) {
        throw "bind failed";
    }
    if (listen(fd, 3) < 0) {
        throw "listen failure";
    }
}

void Server::start(ClientHandler &ch) throw(const char *) {
    // handle a client with no thread
    cout << "waiting for client" << endl;
    // accept a client
    socklen_t clientSize = sizeof(client);
    int aClient = accept(fd, (struct sockaddr *) &client, &clientSize);
    if (aClient < 0) {
        throw "accept failed";}
    cout << "client connected" << endl;
    // handle the client
    ch.handle(aClient);
    cout << "client disconnected" << endl;
    close(aClient);

}
void Server::stop() {
    run = false;
    try {
        t->join();
    } catch (exception &exception) {}
}

// destructor
Server::~Server() {
    if (close(this->fd) < 0) {
        perror("Error closing socket");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

// get the message from the client
basic_string<char> Server::getInfoFromClient() {
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    string result;
    char buffer[4096];
    // loop to get all the data from the client
    while (true) {
        // clear the buffer
        bzero(buffer, 4096);
        // read data from the client
        int read_bytes;
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if ((read_bytes = recv(this->fd, buffer, 4096, 0)) < 0) {
            perror("Error reading from socket");
            return "Error";
            // if we got 0 bytes, the client closed the connection
        } else if (read_bytes == 0) {
            cout << "connection is closed" << endl;
            return "-1";
        } else {
            result = buffer;
            cout << ("input is: " + result) << endl;
            // return the client message for the next step
            return result;
        }
    }
}


// send to a client the class of the vector - the result of classify distance_metric
void Server::sendToClient(string result) {
    const char *dataToSend;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int bytesWritten = 0, totalBytesWritten = 0;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (this->fd < 0) {
        perror("Client's socket not initialized!! Need to call getInfoFromClient() first");
    }
    dataToSend = result.c_str();
    int dataToSendLength = result.size();
    cout << "Data to send to client: " << dataToSend << endl;
    if (write(this->fd, dataToSend,
              dataToSendLength) < 0) {
        perror("Error writing to socket");
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

// this distance_metric splits the string to 3 parts - the vector, the distance metric and the k
void Server::parseStringFromUser(string str, string &vector_str, string &k, string &function) {
    // loop from the end of str to the first " " and save the k
    int i = str.size() - 1;;
    while (str[i] != ' ' and i >= 0) {
        i--;
    }
    // if we reached the end of the string and didn't find a " " - the string is invalid
    if (i == 0) {
        cout << "Error in input" << endl;
        return;
    }
    k = str.substr(i + 1, str.size() - i);
    i--;
    int j = i;
    // loop from the end of str to the second " " and save the distance_metric
    while (str[i] != ' ' && i > 0) {
        i--;
    }
    // if we reached the end of the string and didn't find a " " - the string is invalid
    if (i < 0) {
        k = "";
        function = "";
        vector_str = "";
        return;
    }
    function = str.substr(i + 1, j - i);
    vector_str = str.substr(0, i);
}

// this distance_metric connects the server to the client
void Server::ConnectToClient() {
    // set the server socket to listen to the port
    if (this->fd < 0) {
        perror("Server's socket not initialized!! Need to call createAndBindSocket() first");

    }
    if (listen(this->fd, 5) < 0) {
        perror("error listening to socket");
    }

    struct sockaddr_in client_sin;
    socklen_t addr_len = sizeof(client_sin);
    // listen for incoming connections
    // accept an incoming connection
    if ((this->fd = accept(this->fd, (struct sockaddr *) &client_sin, &addr_len)) < 0) {
        perror("error accepting connection");
    }
    // log the clients IP address
    cout << "Client connected from " << inet_ntoa(client_sin.sin_addr) << ":" << ntohs(client_sin.sin_port) << endl;
}

// this distance_metric connects the server to the client and gets the message from the client
// and sends the result to the client
void Server::runServer(vector<vector<double>> data_vectors, vector<string> labels) {
    this->ConnectToClient();

    while (1) {
        // get the message from the client
        string client_message = this->getInfoFromClient();
        // if the client closed the connection - break the loop and wait to connect to another client
        if (client_message == "-1") {
            break;
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (client_message == "Error") {
            continue;
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        string vector_str, k_temp, function;
        // split the string to 3 parts - the vector, the distance metric and the k
        this->parseStringFromUser(client_message, vector_str, k_temp, function);
        try {
            vector<double> vector_double;
            // convert the vector from string to vector<double>
            // check if the vector is valid
            if (!validityCheck(vector_str, vector_double)) {
                cout << "invalid vector" << endl;
                this->sendToClient("invalid vector");
                continue;
            }
            // check if distance is valid
            if (!isDistanceValid(function)) {
                this->sendToClient("invalid distance distance_metric");
                continue;
            }
            // check if k is valid
            if (!isKValid(k_temp, data_vectors.size())) {
                this->sendToClient("invalid k");
                continue;
            }
            int k = stoi(k_temp);
            if (vector_double.size() != data_vectors[0].size()) {
                this->sendToClient("invalid vector");
                continue;
            }
            Knn knn(k, data_vectors, labels, function);
            string result = knn.classify(vector_double);
            // return the result to the client
            this->sendToClient(result);
            cout << "reply sent to client" << endl;
            continue;

        }
        catch (exception &e) {
            cout << "error: invalid input" << endl;
            this->sendToClient("invalid input");
        }
    }
}

// our client will take care of few clients in parallel. each moment the server can communicate with few clients at the same time.
// when client is connected to the server, the server will send the menu to the client and wait for the client to choose an option.
// the client will send the option to the server and the server will send the result to the client.
// the client will print the result and send the menu to the client again.
// the client will continue to send the menu to the client until the client will choose to exit.
// the client will send the exit option to the server and the server will close the connection with the client.
// the client will continue to send the menu to the client until the client will choose to exit.
int main(int argc, char **argv) {
    // receive the args from the command line:
    // server.out port
    if (2 != argc) {
        cout << "invalid number of arguments" << endl;
        return 0;
    }
//
//     make the Data set for the server:
//    // init the raw input from the file into a vector of vectors of strings
//    vector<vector<string>> input_data_classified;
//    // init double vector of vectors - in order to fill it if the data is valid
//    vector<vector<double>> data_vectors;
//    // init vector of strings - in order to fill it if the data is valid
//    vector<string> dataRows;
//    // init vector of strings - in order to keep the labels from the classified file
//    vector<string> labels;
//    // do try and catch for the file
//    string file = argv[1];
//    // init a char array of the reply from the client
//    char reply[4096];
//
//    // read data from classified file
//    try {
//        ReadDataFromCSV readDataFromCSV(argv[1]);
//        input_data_classified = readDataFromCSV.readData();
//        // take the last element of each vector in input_data_classified and put it into labels
//        for (int i = 0; i < input_data_classified.size(); i++) {
//            labels.push_back(input_data_classified[i].back());
//            input_data_classified[i].pop_back();
//        }
//    }
//    catch (const std::exception &e) {
//        std::cerr << e.what() << '\n';
//    }
//    // change the data to a vector of strings each string is a vector
//    vectorsIntoRows(input_data_classified, dataRows);
//    // check the validity of the data size
//    if (!checkDataSize(input_data_classified)) {
//        // assign the error message to the reply
//        strcpy(reply, "Error: invalid input-Data size");
//        return 0;
//    }
//    if (!checkVecStr(labels)) {
//        strcpy(reply, "Error: invalid input - check labels");
//    }
//    if (dataRows.empty()) {
//        strcpy(reply, "Error: invalid input-Data is empty");
//    }
//
//    for (int i = 0; i < dataRows.size(); i++) {
//        vector<double> temp;
//        validityCheck(dataRows[i], temp);
//        data_vectors.push_back(temp);
//    }
//
//    Dataset dataset(data_vectors, labels);
    // init the server
    char *end;
    Server server(strtol(argv[1], &end, 10));
    if (end == argv[1] || *end != '\0') {
        cout << "invalid port" << endl;
        return 0;
    }
    // run the server - the server will run infinitely
    // the server will wait for a client to connect
    try {
        while (true) {
            // run the server
            KnnHandler knnHandler;
            server.start(knnHandler);
        }
    }

    catch (exception &e) {
        cout << "error: server stopped running" << endl;
        return 0;
    }

}
