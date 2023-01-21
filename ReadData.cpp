#include <string>
#include "ReadData.h"
using namespace std;

// Constructor
class ReadDataFromCSV : public ReadData {
public:
    ReadDataFromCSV(const string fileName) {
        this->fileName = fileName;
    }

    // Read data from CSV file
    vector<vector<string> > readData() {
        vector<vector<string> > data;
        string line;
        ifstream file(this->fileName);

        // check if file is open
        // if so, read data into rows
        if (file.is_open()) {
            while (getline(file, line)) {
                vector<string> row;
                string word = "";
                for (auto x : line) {
                    if (x == ',') {
                        row.push_back(word);
                        word = "";
                    } else {
                        word = word + x;
                    }
                }
                row.push_back(word);
                data.push_back(row);
            }
            file.close();
        }
            // exception handling
        else throw invalid_argument("file is not valid");
        return data;
    }
};
