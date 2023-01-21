//
// Created by User on 16/01/2023.
//

#include "CLI.h"
#include "commands.h"

CLI::CLI(DefaultIO *dio) {
    this->dio = dio;
}
// start CLI by creating CLIData instance and executing commands in loop
void CLI::start() {
    // create a new bool to indicate if the cli is running
    bool *running=new bool;
    *running = true;
    // create a new CLIData instance to hold the data of the cli
    CLIData *cliData = new CLIData(*dio, running);
    // create a float to read the selected_option from the client
    float selected_option;
    // while running is true read option from client and execute the command
    while (*running) {
        cout << "cli started" << endl;
        cliData->printMenu();
        cout << "menu printed" << endl;
        // read the selected option from the client
        dio->read(&selected_option);
        cout << "selected option: " << selected_option << endl;
        // execute the command
        cliData->executeCommand((int) selected_option);
        cout << "command executed" << endl;
    }
    // delete the allocated memory
    delete running;
    delete cliData;
    delete dio;
}

CLI::~CLI() {

}