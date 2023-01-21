//
// Created by User on 14/01/2023.
//

#ifndef ASS4AP_TESTDATA_H
#define ASS4AP_TESTDATA_H


#include <string>

#include "ReadData.h"
#include "Dataset.h"
#include "CheckValid.h"
#include "TrainData.h"


class TestData : public Dataset {
public:
    TestData();

    explicit TestData(string path){
        initialize(path);
    }

    void initialize(string file);

    void classifyData(TrainData &trainData, int k, string function);
};


#endif //ASS4AP_TESTDATA_H
