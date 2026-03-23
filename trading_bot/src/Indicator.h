#pragma once
#include "MarketData.h"
#include <vector>

using namespace std;

class Indicator {
public:
    // Calculates Simple Moving Average backward from currentIndex spanning windowSize
    static double calculateSMA(const vector<DataPoint>& data, int currentIndex, int windowSize);
    
    // Calculates standard deviation of prices backward from currentIndex spanning windowSize
    static double calculateVolatility(const vector<DataPoint>& data, int currentIndex, int windowSize);
    
    // Checks if the price dropped significantly over a lookback period
    static bool priceRecentlyDropped(const vector<DataPoint>& data, int currentIndex, int lookback);

    // Calculates Exponential Moving Average
    static double calculateEMA(const vector<DataPoint>& data, int currentIndex, int windowSize);

    // Calculates Relative Strength Index
    static double calculateRSI(const vector<DataPoint>& data, int currentIndex, int windowSize);

    // Calculates MACD (Moving Average Convergence Divergence)
    static double calculateMACD(const vector<DataPoint>& data, int currentIndex, int shortWindow, int longWindow);
};
