//
// Created by Sigal Graboys 319009304 on 13/01/2023
// and Yair Pickholtz Berliner ???? on 13/01/2023
//

#ifndef ASS4AP_COMMANDS_H
#define ASS4AP_COMMANDS_H

#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <map>
#include <utility>
#include <fstream>
#include <iomanip>
#include <sys/socket.h>
#include <sys/socket.h>
#include "Dataset.h"
#include "TrainData.h"
#include "TestData.h"


using namespace std;

class DefaultIO {
public:
    virtual string read() = 0;

    virtual void write(string text) = 0;

    virtual void write(float f) = 0;

    virtual void read(float *f) = 0;

    virtual ~DefaultIO() {}
};

// DefaultIO interface implementation - standard input/output (console)
class StandardIO : public DefaultIO {
public:
    StandardIO() : DefaultIO() {}

    virtual string read() {
        string str;
        cin >> str;
        return str;
    }

    // write a string to the default output
    virtual void write(string text) {
        cout << text;
    }

    // write a float number to the default output
    virtual void write(float f) {
        cout << f;
    }

    virtual void read(float *f) {
        cin >> *f;
    }

    ~StandardIO() {}
};

// SocketIO interface implementation - using sockets
class SocketIO : public DefaultIO {
    int client_socket;
public:
    // constructor
    SocketIO(int client_socket) noexcept: DefaultIO(), client_socket(client_socket) {}

    // read a line from the socket
    string read() override {
        /*string line;
        char c = 0;
        // read until the end of the line - '\n'
        recv(this->client_socket, &c, sizeof(char), 0);
        while (c != '\n' || c != '\r') {
            cout << "in read" << endl;
            cout << c << endl;
            line += c;
            recv(this->client_socket, &c, sizeof(char), 0);
        }
        return line;
        cout << "after read" << endl;
        cout << line << endl;*/
        string line="";
        char c = 0;
        //read from socket until encounter '\n' character
        recv(this->client_socket, &c, sizeof(char), 0);
        while (c != '\n' && c != '\r') {
            line += c;
            recv(this->client_socket, &c, sizeof(char), 0);
        }
        return line;
    }

    void read(float *f) override {
        string buffer = read();
        *f = stof(buffer);
    }

    void write(string text) override {
        send(this->client_socket, text.c_str(), text.size(), 0);
    }

    void write(float f) override {
        ostringstream oss;
        oss << f;
        string text(oss.str());
        write(text);
    }
};

class Command {
protected:
    DefaultIO &dio;
public:
    string description;
    // constructor

    Command(DefaultIO &dio, const string &s) : dio(dio), description(s) {}

    virtual void execute() = 0;

    virtual ~Command() {} // destructor
};

class Upload : public Command {
private: // private members
    TrainData &trainData;
    TestData &testData;
    int *numOflines;
public:
    explicit Upload(DefaultIO &dio, const string &s, TrainData &TrainData, TestData &testData, int *numOflines) :
            Command(dio, s), trainData(TrainData), testData(testData), numOflines(numOflines) {}

    // handle getting two csv files from the user
    virtual void execute() {
        // read the first file - train data
        dio.write("Please upload your local train CSV file.\n");
        while (true) {
            string line = dio.read();
            if (line.empty()) {
                break;
            }
            trainData.addVector(line);
        }
        string filePath = dio.read();
        cout << filePath << endl;
        this->trainData.initialize(filePath);
        dio.write("Upload complete.\n");
        // read the second file - test data
        dio.write("Please upload your local test CSV file.\n");
        filePath = dio.read();
//        readFile(filePath);
        this->testData.initialize(filePath);
        *numOflines = (int) this->testData.get_X_Samples().at(0).size();
        dio.write("Upload complete.\n");
    }

    virtual ~Upload() {}
};

class algorithmSettings : public Command {
private:
    // k - the number of neighbors
    int k;
    // distance metric
    string distance_metric;
public:
    explicit algorithmSettings(DefaultIO &dio, const string &menu_description, int k, string distance_metric) :
            Command(dio, menu_description), k(k), distance_metric(distance_metric) {}

    // this distance_metric splits the string to 2 parts - distance metric and the k
    static void parseStringFromUser(string str, string &k, string &distance) {
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
        distance = str.substr(i + 1, str.size() - i);
        i--;
        int j = i;
        // loop from the end of str to the second " " and save the distance_metric
        while (str[i] != ' ' && i > 0) {
            i--;
        }
        // if we reached the end of the string and didn't find a " " - the string is invalid
        if (i < 0) {
            k = "";
            distance = "";
            return;
        }
        k = str.substr(i + 1, j - i);
    }

    // handle changing the settings of the program - k and distance metric
    virtual void execute() {
        // print the current settings
        dio.write("The current KNN parameters are: K = " + to_string(k) + ", distance metric = " + distance_metric + "\n");
        // read the new settings from the user
        string user_input;
        string new_K_string, new_distance_metric;
        bool flag;
        do {
            user_input = dio.read();
            if (user_input.empty()) {
                break;
            }
            parseStringFromUser(user_input, new_K_string, new_distance_metric);
            // if the input is invalid - print an error message
            if (isKValid(new_K_string) || isDistanceValid(new_distance_metric)) {
                dio.write("Error in input, please try again\n");
                flag = true;
            }
                // if the input is valid - print a success message
            else {
                dio.write("New settings have been set\n");
                flag = false;
            }
        } while (flag);
        // update the settings
        this->k = stoi(new_K_string);
        this->distance_metric = new_distance_metric;
    }
    virtual ~algorithmSettings() {}
};

//Exit holds pointer to bool variable that determine whether to loop in the CLI should stop.
class Exit : public Command {
    bool *running;
public:
    Exit(DefaultIO &dio, const string &s, bool *b) : Command(dio, s), running(b) {}

    // handle exiting the program - set the running variable to false
    void execute() override {
        *running = false;
    }
};

class CLIData {
private:
    DefaultIO &dio;
    vector<Command *> commands{6};
    vector<string> labels{};
    TrainData *trainData=new TrainData();
    TestData *testData=new TestData();
    int *k = new int;
    string *function = new string;
    int *numOflines = new int;
public:
    // constructor sends
    CLIData(DefaultIO &d, bool *r) : dio(d) {
        // initialize default values
        *k = 5;
        *function = "EUC";
        // initialize the commands
        commands[0] = new Upload(d, "1. upload an unclassified csv data file", *trainData, *testData, numOflines);
        commands[1] = new algorithmSettings(d, "2. algorithm settings", *k, *function);
        commands[2] = new algorithmSettings(d, "2. algorithm settings", *k, *function);
        commands[3] = new algorithmSettings(d, "2. algorithm settings", *k, *function);
        commands[4] = new algorithmSettings(d, "2. algorithm settings", *k, *function);
        //commands[2] = new classifyData(d, "3. classify data", *trainData, *testData, *k, *distance_metric, *numOflines);
        //commands[3] = new displayResults(d, "4. display results", *trainData, *testData, *k, *distance_metric, *numOflines);
        //commands[4] = new downloadResults(d, "5. download results", *trainData, *testData, *k, *distance_metric, *numOflines);
        commands[5] = new Exit(d, "8. exit", r);
    }

    void executeCommand(int i) {
        switch (i) {
            case 1: commands[0]->execute();
                break;
            case 2: commands[1]->execute();
                break;
            case 3: commands[2]->execute();
                break;
            case 4: commands[3]->execute();
                break;
            case 5: commands[4]->execute();
                break;
            case 8:
                commands[5]->execute();
                break;
            default:
                std::cout << "Error: Invalid value" << std::endl;
                commands[5]->execute();
                break;
        }
    }
    void printMenu() {
        // print the menu to the user
        dio.write("Welcome to the KNN classifier Server. Please choose an option:\n");
        for (Command *command: commands) {
            dio.write(command->description+"\n");
        }
    }
    ~CLIData() {
        delete trainData;
        delete testData;
        delete k;
        delete function;
        delete numOflines;
        for (Command *command: commands) {
            delete command;
        }
    }


};


#endif //ASS4AP_COMMANDS_H
