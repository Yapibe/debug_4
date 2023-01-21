#ifndef AP_ASS2_KNN_H
#define AP_ASS2_KNN_H

#include <algorithm>
#include <map>
#include <utility>
#include <vector>
#include <string>
#define MINKOWSKI_ORDER  2.0;
#include <cmath>
#include <iostream>

using namespace std;

class Knn {

public:
    Knn(int k, vector<vector<double> > &X_train, vector<string> &Labels, string &distance_function);
    string classify(const vector<double>& X_test);
private:
    int k;
    vector<vector<double> > X_train;
    vector<string> Labels;
    string distance_function;
};
#endif //AP_ASS2_KNN_H
