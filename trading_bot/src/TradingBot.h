#pragma once
#include "Portfolio.h"
#include "MarketData.h"
#include <vector>
#include <string>

using namespace std;
struct TradeRecord {
    int dayIndex;
    string type; // "BUY" or "SELL"
    double price;
    int shares;
    string reason; // "AI", "Stop-Loss", or "Take-Profit"
};

class TradingBot {
public:
    TradingBot(double initial_balance, int short_window, int long_window);
    
    void train(const MarketData& marketData, int epochs);
    void test(const MarketData& marketData);
    void printResults(const MarketData& marketData) const;

private:
    Portfolio portfolio;
    // Q-Learning state variables
    double qTable[96][3];
    double alpha;
    double gamma;
    double epsilon;
    
    int previousState;
    int previousAction;
    double previousPortfolioValue;

    int getStateId(bool holding, bool trend, bool drop, bool vol, int rsiState, bool macd) const;
    void executeEpoch(const MarketData& marketData, bool isTraining, double currentEpsilon);
    vector<TradeRecord> tradeHistory;
    
    int shortWindow;
    int longWindow;
    int volatilityWindow;
    double initialBalance;


};
