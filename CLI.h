//
// Created by User on 16/01/2023.
//

#ifndef ASS4AP_CLI_H
#define ASS4AP_CLI_H


#include <cstring>
#include "commands.h"

using namespace std;

class CLI {
    DefaultIO *dio;
public:
    CLI(DefaultIO *dio);

    void start();

    virtual ~CLI();
};


#endif //ASS4AP_CLI_H
