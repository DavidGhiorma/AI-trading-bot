#include "MarketData.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

MarketData::MarketData(const string& filename) : filename(filename) {}

bool MarketData::loadData() {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return false;
    }

    string line;
    // Skip the header line
    if (getline(file, line)) {
        // Assume header is timestamp,price
    }

    while (getline(file, line)) {
        // Skip empty lines
        if (line.empty()) continue;

        stringstream ss(line);
        string timestampStr, priceStr;
        
        if (getline(ss, timestampStr, ',') && getline(ss, priceStr, ',')) {
            try {
                double price = stod(priceStr);
                data.push_back({timestampStr, price});
            } catch (const exception& e) {
                cerr << "Warning: Invalid price syntax for value: " << priceStr << endl;
            }
        }
    }
    
    file.close();
    return !data.empty();
}

const vector<DataPoint>& MarketData::getData() const {
    return data;
}
