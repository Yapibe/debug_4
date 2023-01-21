#include "Knn.h"
#include "CheckValid.h"

////////////////////////////////////////////////////////////////////////////////////////
// Distance functions
double euclideanDistance(const vector<double> &x1, const vector<double> &x2);
double manhattanDistance(const vector<double> &x1, const vector<double> &x2);
double minkowskiDistance(const vector<double> &x1, const vector<double> &x2);
double chevishevDistance(const vector<double> &vector_1, const vector<double> &vector_2);
double canberraDistance(const vector<double> &x1, const vector<double> &x2);

// First distance distance_metric - Euclidean Function

double euclideanDistance(const vector<double> &vector_1, const vector<double> &vector_2) {
    double sum = 0;
    for (int i = 0; i < vector_1.size(); i++) {
        sum += pow(vector_2.at(i) - vector_1.at(i), 2);
    }
    return sqrt(sum);
}

//Second distance distance_metric - Manhattan Function

double manhattanDistance(const vector<double> &vector_1, const vector<double> &vector_2) {
    double sum = 0;
    for (int i = 0; i < vector_1.size(); i++) {
        sum += abs(vector_2.at(i) - vector_1.at(i));
    }
    return sum;
}

//Third distance distance_metric - Chevishev Function

double chevishevDistance(const vector<double> &vector_1, const vector<double> &vector_2) {
    double sum = 0;
    double max = 0;
    for (int i = 0; i < vector_1.size(); i++) {
        sum = abs(vector_2.at(i) - vector_1.at(i));
        if (sum > max) {
            max = sum;
        }
    }
    return max;
}

//Fourth distance distance_metric - Canberra Function

double canberraDistance(const vector<double> &vector_1, const vector<double> &vector_2) {
    double sum = 0;
    for (int i = 0; i < vector_1.size(); i++) {
        if (vector_1[i] == 0 & vector_2[i] == 0) {
            std::cout << " cant divide by zero";
        } else {
            double denominator = abs(vector_2[i]) + abs(vector_1[i]);
            sum += (abs(vector_2.at(i) - vector_1.at(i))) / denominator;
        }
    }
    return sum;
}

//Fifth distance distance_metric - Minkowski Function

double minkowskiDistance(const vector<double> &vector_1, const vector<double> &vector_2) {
    double sum = 0;
    double minkowski_order = MINKOWSKI_ORDER;

    for (int i = 0; i < vector_1.size(); i++) {
        sum += pow(abs(vector_2.at(i) - vector_1.at(i)), minkowski_order);
    }
    return pow(sum, (1.0) / (minkowski_order));
}
////////////////////////////////////////////////////////////////////////////////////////
// create knn class for knn algorithm

// constructor
Knn::Knn(int k, vector<vector<double>> &X_train, vector<string> &Labels, string &distance_function){
    this -> k = k;
    this -> X_train = X_train;
    this -> Labels = Labels;
    this -> distance_function = distance_function;
}

////////////////////////////////////////////////////////////////////////////////////////
// methods

// create classify distance_metric
string Knn::classify(const vector<double>& X_test) {
    // create map for distances
    map<string, int> myMap = {{"AUC", 1}, {"MAN", 2}, {"CHB", 3}, {"CAN", 4}, {"MIN", 5}};

    // create query that accepts pairs of distance and label
    vector<pair<double, string>> distances;
    //switch case for distance distance_metric
    switch (myMap[distance_function]) {
        case 1:
            // create for loop to iterate through X_train
            for (int i = 0; i < X_train.size(); i++) {
                // create a pair of distance and label
                pair<double, string> p = make_pair(euclideanDistance(X_test, X_train.at(i)), Labels.at(i));
                // push pair to query
                distances.push_back(p);
            }
            break;
        case 2:
            // create for loop to iterate through X_train
            for (int i = 0; i < X_train.size(); i++) {
                // create a pair of distance and label
                pair<double, string> p = make_pair(manhattanDistance(X_test, X_train.at(i)), Labels.at(i));
                // push pair to query
                distances.push_back(p);
            }
            break;
        case 3:
            // create for loop to iterate through X_train
            for (int i = 0; i < X_train.size(); i++) {
                // create a pair of distance and label
                pair<double, string> p = make_pair(chevishevDistance(X_test, X_train.at(i)), Labels.at(i));
                // push pair to query
                distances.push_back(p);
            }
            break;
        case 4:
            // create for loop to iterate through X_train
            for (int i = 0; i < X_train.size(); i++) {
                // create a pair of distance and label
                pair<double, string> p = make_pair(canberraDistance(X_test, X_train.at(i)), Labels.at(i));
                // push pair to query
                distances.push_back(p);
            }
            break;
        case 5:
            // create for loop to iterate through X_train
            for (int i = 0; i < X_train.size(); i++) {
                // create a pair of distance and label
                pair<double, string> p = make_pair(minkowskiDistance(X_test, X_train.at(i)), Labels.at(i));
                // push pair to query
                distances.push_back(p);
            }
            break;
    }
    sort(distances.begin(), distances.end());
    // create map to store labels and their frequency
    map<string, int> frequency;
    for (int i = 0; i < this->k; i++) {
        frequency[distances[i].second]++;
    }
    // create iterator to iterate over the map
    map<string, int>::iterator it;
    // create string to store the label with the highest frequency
    string label;
    int max = 0;
    for (it = frequency.begin(); it != frequency.end(); it++) {
        if (it->second > max) {
            max = it->second;
            label = it->first;
        }
    }
    return label;
}




