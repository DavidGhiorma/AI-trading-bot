#include "Indicator.h"
#include <cmath>

using namespace std;

double Indicator::calculateSMA(const vector<DataPoint>& data, int currentIndex, int windowSize) {
    if (currentIndex < windowSize - 1) return 0.0;
    
    double sum = 0.0;
    for (int i = 0; i < windowSize; ++i) {
        sum += data[currentIndex - i].price;
    }
    return sum / windowSize;
}

double Indicator::calculateVolatility(const vector<DataPoint>& data, int currentIndex, int windowSize) {
    if (currentIndex < windowSize - 1) return 0.0;
    
    double mean = calculateSMA(data, currentIndex, windowSize);
    double variance = 0.0;
    
    for (int i = 0; i < windowSize; ++i) {
        double diff = data[currentIndex - i].price - mean;
        variance += diff * diff;
    }
    
    variance /= windowSize;
    return sqrt(variance);
}

bool Indicator::priceRecentlyDropped(const vector<DataPoint>& data, int currentIndex, int lookback) {
    if (currentIndex < lookback) return false;
    
    double currentPrice = data[currentIndex].price;
    double pastPrice = data[currentIndex - lookback].price;
    
    // We arbitrarily define a "recent drop" as at least a 2% depreciation compared to lookback days ago
    return currentPrice < (pastPrice * 0.98); 
}

double Indicator::calculateEMA(const vector<DataPoint>& data, int currentIndex, int windowSize) {
    if (currentIndex < windowSize) return calculateSMA(data, currentIndex, windowSize);
    
    double multiplier = 2.0 / (windowSize + 1.0);
    // Start SMA from the very beginning of the usable window
    double ema = calculateSMA(data, windowSize - 1, windowSize);
    
    for (int i = windowSize; i <= currentIndex; ++i) {
        ema = (data[i].price - ema) * multiplier + ema;
    }
    return ema;
}

double Indicator::calculateRSI(const vector<DataPoint>& data, int currentIndex, int windowSize) {
    if (currentIndex < windowSize) return 50.0; // Default neutral RSI
    
    double gain = 0.0, loss = 0.0;
    
    for (int i = currentIndex - windowSize + 1; i <= currentIndex; ++i) {
        double change = data[i].price - data[i - 1].price;
        if (change > 0) gain += change;
        else loss -= change;
    }
    
    if (loss == 0) return 100.0; // No losses means perfectly overbought
    
    double rs = (gain / windowSize) / (loss / windowSize);
    return 100.0 - (100.0 / (1.0 + rs));
}

double Indicator::calculateMACD(const vector<DataPoint>& data, int currentIndex, int shortWindow, int longWindow) {
    if (currentIndex < longWindow - 1) return 0.0;
    
    double shortEMA = calculateEMA(data, currentIndex, shortWindow);
    double longEMA = calculateEMA(data, currentIndex, longWindow);
    
    return shortEMA - longEMA;
}
