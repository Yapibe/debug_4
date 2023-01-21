#ifndef AP_ASS2_READDATA_H
#define AP_ASS2_READDATA_H
#include "iostream"
#include "fstream"

#include <vector>
#include <sstream>
#include <stdexcept>
using namespace std;

class ReadData {
protected:
    std::ifstream file;
    std::string fileName;
};



#endif //AP_ASS2_READDATA_H
