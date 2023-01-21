//
// Created by User on 14/01/2023.
//

#include "TestData.h"
#include "ReadData.cpp"
#include "TrainData.h"
#include "Knn.h"

void TestData::classifyData(TrainData &trainData, int k = 5, string function = "AUC") {
    vector<vector<double>> attribute_vector = this->get_X_Samples();
    vector<string> labels_vector = this->get_Labels();

    Knn knn(k, attribute_vector, labels_vector, function);
    for (int i=0; i < attribute_vector.size(); i++) {
        this->Labels.push_back(knn.classify(attribute_vector[i]));
    }
}


void TestData::initialize(string file) {
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
    // read data from classified file
    try {
        ReadDataFromCSV readDataFromCSV(file);
        input_data_classified = readDataFromCSV.readData();
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
    // change the data to a vector of strings each string is a vector
    vectorsIntoRows(input_data_classified, dataRows);
    // check the validity of the data size
    if (!checkDataSize(input_data_classified)) {
        // assign the error message to the reply
        throw invalid_argument("Error: invalid input-Data size") ;
    }
    // check the validity of the data size
    if (dataRows.empty()) {
        throw invalid_argument( "Error: invalid input-Data is empty");
    }

    for (int i = 0; i < dataRows.size(); i++) {
        vector<double> temp;
        validityCheck(dataRows[i], temp);
        this->X_Samples.push_back(temp);
    }

//    if (!checkVecStr(this->Labels)) {
//        return "Error: invalid input - check labels";
//    }

}

TestData::TestData() {
    //cout<<"TestData constructor"<<endl;
}


