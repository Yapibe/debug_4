#include "CheckValid.h"

bool isKValid(string argv, int data_size) {
    // check if the input is valid - if k is a positive integer
    string k_temp = argv;
    for (int i = 0; i < k_temp.length(); i++) {
        if (!isdigit(k_temp[i])) {
            cout << "k should be a positive integer" << endl;
            return 0;
        }
    }
    // check that k is in the range of the data size.
    if (atoi(k_temp.c_str()) > data_size) {
        cout << "k should be smaller than the number of samples" << endl;
        return 0;
    }
    return 1;
}
bool isKValid(string argv) {
    // check if the input is valid - if k is a positive integer
    string k_temp = argv;
    for (int i = 0; i < k_temp.length(); i++) {
        if (!isdigit(k_temp[i])) {
            cout << "k should be a positive integer" << endl;
            return 0;
        }
    }
}


//Check if the input strings is valid
bool checkDataSize(vector<vector<string>> &dataRows) {

    int vector_size = dataRows[0].size();
    for (int i = 0; i < dataRows.size(); i++) {
        if (dataRows[i].size() != vector_size) {
            return false;
        }
    }
    return true;
}

//Check if the query of strings is valid
bool checkVecStr(std::vector<std::string> labels) {
    if (labels.size() == 0) {
        return false;
    }

    for (int i = 0; i < labels.size(); i++) {
        if (labels[i].length() == 0) {
            return false;
        }
    }
    return true;
}

// check if the query ended
bool endCheck(char str) {
    return (str == ' ');
}

// check is floater is correct
bool floaterCheck(char str, bool floater) {
    return (str == '.' && !floater);
}


// change the data from a vector of vectors of strings to a vector of strings
void vectorsIntoRows(std::vector<std::vector<std::string>> data, vector<string> &dataRows) {
    string row = "";
    for (int i = 0; i < data.size(); i++) {
        for (int j = 0; j < data[i].size(); j++) {
            row += data[i][j];
            if (j != data[i].size() - 1) {
                row += " ";
            }
        }
        dataRows.push_back(row);
        row = "";
    }
}
// check if the string is valid
void ValidCheckVectors(vector<string> &dataRows,
                       std::vector<std::vector<double>> &output_vectors) {
    string row = "";
    vector<string> rows;
    // for each string in data query of vectors of strings - check if it is valid
    for (int i = 0; i < dataRows.size(); i++) {
        for (int j = 0; j < dataRows[i].size(); j++) {
            row += dataRows[i][j];
            row += " ";
        }
        rows.push_back(row);
    }
    for (int i = 0; i < dataRows.size(); i++) {
        validityCheck(dataRows[i], output_vectors.at(i));
    }
}

// check if distance is valid
bool isDistanceValid(string distance) {
    // check if the distance distance_metric is valid

    if (distance == "MAN" || distance == "AUC" || distance == "CHB" || distance == "CAN" || distance == "MIN") {
        return true;
    }
    return false;
}

// check we get a valid number by
bool validityCheck(string input, vector<double> &vector) {
    bool floater = false;
    // check if received input is empty
    if (input.empty()) {
        cout << "one of the vectors is empty"<< endl;
        return false;
    }

    // check for consecutive spaces
    for (int i = 0; i < input.length(); i++) {
        if (input[i] == ' ' && input[i + 1] == ' ') {
            cout << "error: invalid input - consecutive spaces"<< endl;
            return false;
        }
    }

    // check if the last char in string is ' ' if so - return error
    if (input.back() == ' ' || input[0] == ' ') {
        cout << "error: invalid input - last char is space"<< endl;
        return false;
        // add space to end to iterate properly
    } else input = input + ' ';

    string intermediate = " ";
    // check if all string characters are valid
    for (int i = 0; i < input.length(); i++) {
        // check if every digit is in bounds
        if (isdigit(input[i])) {
            // if so, add to intermediate
            intermediate += input[i];
        }
            // check if number is a minus, make sure the '-' is followed by a digit
        else if ((intermediate == " " && input[i] == '-') && isdigit(input[i + 1])) {
            intermediate += input[i];
        }
            //if not in bounds, check if point for float
        else if (floaterCheck(input[i], floater)) {
            // if so, add to intermediate
            intermediate += input[i];
            // change flag to show we have a point floater
            floater = true;
        }
            //check if we reached end of the string
        else if (endCheck(input[i])) {
            // if we reach end, turn into query and add to query array

            // check if the number is in the right range
            try {
                vector.push_back(stof(intermediate));
                intermediate = " ";
                floater = false;
            }
            catch (std::out_of_range &) {
                cout << "invalid input - catch check valid" << endl;
                exit(0);
            }


        }
            //if not valid print error message
        else {
            cout << "error: invalid input - ?????" << endl;
            return false;
        }
    }
    //continue with input checking
    return true;
}