#ifndef AP_ASS2_DATASET_H
#define AP_ASS2_DATASET_H
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Dataset {
public:
    Dataset(vector<vector<double> > attribute_vector, vector<string> labels_vector);

    Dataset();

    vector<vector<double> > get_X_Samples();

    vector<string> get_Labels();
    void print_X_samples();
    void print_Labels();

protected:
    vector<vector<double> > X_Samples;
    vector<string> Labels;
};


#endif //AP_ASS2_DATASET_H