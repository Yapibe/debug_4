
#ifndef ASS4AP_TRAINDATA_H
#define ASS4AP_TRAINDATA_H

#include "ReadData.h"
#include "Dataset.h"
#include "CheckValid.h"


class TrainData : public Dataset {

public:

    TrainData();
    explicit TrainData(string path){
        initialize(path);
    }

    void initialize(string file);

    void addVector(string line);
};


#endif //ASS4AP_TRAINDATA_H
