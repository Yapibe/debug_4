//
// Created by User on 14/01/2023.
//

#include "TrainData.h"
#include "ReadData.cpp"

void TrainData::addVector(string line) {
    vector<double> temp;
    validityCheck(line, temp);
    this->X_Samples.push_back(temp);
}


void TrainData::initialize(string file) {
    // make the Data set for the server:
    // init the raw input from the file into a vector of vectors of strings
    vector<vector<string>> input_data_classified;
    // init double vector of vectors - in order to fill it if the data is valid
    vector<vector<double>> data_vectors;
    // init vector of strings - in order to fill it if the data is valid
    vector<string> dataRows;
    // init vector of strings - in order to keep the labels from the classified file
    vector<string> labels;
    // do try and catch for the file
//    string file = file;
    // read data from classified file
    try {
        ReadDataFromCSV readDataFromCSV(file);
        input_data_classified = readDataFromCSV.readData();
        // take the last element of each vector in input_data_classified and put it into labels
        for (int i = 0; i < input_data_classified.size(); i++) {
            this->Labels.push_back(input_data_classified[i].back());
            input_data_classified[i].pop_back();
        }
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
    // change the data to a vector of strings each string is a vector
    vectorsIntoRows(input_data_classified, dataRows);
    // check the validity of the data size
    if (!checkDataSize(input_data_classified)) {
        // assign the error message to the reply
        throw invalid_argument("Error: invalid input-Data size");
    }
    if (!checkVecStr(this->Labels)) {
        throw invalid_argument( "Error: invalid input - check labels");
    }
    if (dataRows.empty()) {
        throw invalid_argument( "Error: invalid input-Data is empty");
    }

    for (int i = 0; i < dataRows.size(); i++) {
        vector<double> temp;
        validityCheck(dataRows[i], temp);
        this->X_Samples.push_back(temp);
    }
}

TrainData::TrainData() {
    //cout<<"TrainData constructor"<<endl;
}







