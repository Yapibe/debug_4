#include "Dataset.h"

// constructor
Dataset::Dataset(vector<vector<double>> attribute_vector, vector<string> labels_vector) {
    this->X_Samples = attribute_vector;
    this->Labels = labels_vector;
}
// getters
vector<vector<double>> Dataset::get_X_Samples() {
    return this->X_Samples;
}

vector<string> Dataset::get_Labels() {
    return this->Labels;
}
// print functions
void Dataset::print_X_samples() {
    for (int i = 0; i < this->X_Samples.size(); i++) {
        for (int j = 0; j < this->X_Samples.at(i).size(); j++) {
            cout << this->X_Samples.at(i).at(j) << " ";
        }
        cout << endl;
    }
}

void Dataset::print_Labels() {
    for (int i = 0; i < this->Labels.size(); i++) {
        for (int j = 0; j < this->Labels.at(i).size(); j++) {
            cout << this->Labels.at(i).at(j) << " ";
        }
        cout << endl;
    }
}

Dataset::Dataset() {
    this->X_Samples = vector<vector<double>>();
    this->Labels = vector<string>();
}




