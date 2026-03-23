#pragma once
#include <string>
#include <vector>

using namespace std;

struct DataPoint {
    string timestamp;
    double price;
};

class MarketData {
public:
    MarketData(const string& filename);
    bool loadData();
    const vector<DataPoint>& getData() const;

private:
    string filename;
    vector<DataPoint> data;
};
