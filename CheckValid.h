#ifndef AP_ASS2_NEW_CHECKVALID_H
#define AP_ASS2_NEW_CHECKVALID_H
# include "fstream"
# include "vector"
#include<iostream>
#include<string.h>
#include <stdexcept>

using namespace std;
bool checkVecStr(std::vector<std::string> labels);
bool endCheck(char str);
bool floaterCheck(char str, bool floater);
bool isDistanceValid(string distance);
void vectorsIntoRows(std::vector<std::vector<std::string> > data, vector<string> &dataRows);
bool checkDataSize(vector<vector<string>> &dataRows);
bool validityCheck(string input, vector<double> &vector);
void ValidCheckVectors(vector<string> &dataRows,
                       std::vector<std::vector<double> > &output_vectors);
bool isKValid(string argv, int data_size);
bool isKValid(string argv);
#endif //AP_ASS2_NEW_CHECKVALID_H
