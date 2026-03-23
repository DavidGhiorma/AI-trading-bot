#include "TradingBot.h"
#include "Indicator.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

TradingBot::TradingBot(double initial_balance, int short_window, int long_window)
    : portfolio(initial_balance), shortWindow(short_window), 
      longWindow(long_window), volatilityWindow(10), 
      initialBalance(initial_balance),
      alpha(0.1), gamma(0.9), epsilon(0.1),
      previousState(-1), previousAction(-1), previousPortfolioValue(initial_balance) {
    for (int i = 0; i < 96; i++) {
        for (int j = 0; j < 3; j++) {
            qTable[i][j] = 0.0;
        }
    }
}

int TradingBot::getStateId(bool holding, bool trend, bool drop, bool vol, int rsiState, bool macd) const {
    return (holding ? 1 : 0) * 48 + 
           (trend ? 1 : 0) * 24 + 
           (drop ? 1 : 0) * 12 + 
           (vol ? 1 : 0) * 6 + 
           rsiState * 2 + 
           (macd ? 1 : 0);
}

void TradingBot::train(const MarketData& marketData, int epochs) {
    if (epochs <= 0) return;
    cout << "\n[AI] Training started for " << epochs << " epochs...\n";
    for (int e = 0; e < epochs; e++) {
        // Linear decay from epsilon to 0.05
        double currentEpsilon = max(0.05, epsilon * (1.0 - (double)e / epochs));
        executeEpoch(marketData, true, currentEpsilon);
        
        // Print progress every 10%
        int printInterval = max(1, epochs / 10);
        if (e == 0 || (e + 1) % printInterval == 0) {
            cout << "  - Epoch " << e + 1 << "/" << epochs << " completed. (Epsilon: " << currentEpsilon << ")\n";
        }
    }
    cout << "[SUCCESS] AI Model trained successfully!\n";
}

void TradingBot::test(const MarketData& marketData) {
    cout << "\n[AI] Testing model on historical data (Exploitation only, Epsilon = 0)...\n";
    executeEpoch(marketData, false, 0.0);
    cout << "[SUCCESS] Testing completed!\n";
}

void TradingBot::executeEpoch(const MarketData& marketData, bool isTraining, double currentEpsilon) {
    const auto& data = marketData.getData();
    if (data.size() < longWindow) {
        if (!isTraining) cerr << "Not enough data points to run simulation.\n";
        return;
    }

    portfolio = Portfolio(initialBalance);
    previousState = -1;
    previousAction = -1;
    previousPortfolioValue = initialBalance;
    if (!isTraining) {
        tradeHistory.clear();
    }
    
    for (size_t i = longWindow; i < data.size(); ++i) {
        double currentPrice = data[i].price;
        
        // Calculate Indicators
        double shortSMA = Indicator::calculateSMA(data, i, shortWindow);
        double longSMA = Indicator::calculateSMA(data, i, longWindow);
        double volatility = Indicator::calculateVolatility(data, i, volatilityWindow);
        double rsi = Indicator::calculateRSI(data, i, 14);
        double macd = Indicator::calculateMACD(data, i, 12, 26);
        bool dropped = Indicator::priceRecentlyDropped(data, i, 3); 

        // Determine State
        bool trendFired = (shortSMA > longSMA);
        bool dropFired = dropped;
        bool volFired = (volatility > (currentPrice * 0.02)); 
        int rsiState = rsi < 30 ? 1 : (rsi > 70 ? 2 : 0);
        bool macdFired = (macd > 0);
        
        int currentState = getStateId(portfolio.getShares() > 0, trendFired, dropFired, volFired, rsiState, macdFired);
        
        // Calculate Reward and Update Q-Table
        double currentPortfolioValue = portfolio.getCash() + portfolio.getShares() * currentPrice;
        if (isTraining && previousState != -1) {
            double reward = currentPortfolioValue - previousPortfolioValue;
            double maxQ = -1e9;
            vector<int> nextValidActions = {0}; 
            if (portfolio.getCash() >= currentPrice) nextValidActions.push_back(1);
            if (portfolio.getShares() > 0) nextValidActions.push_back(2);
            for (int a : nextValidActions) maxQ = max(maxQ, qTable[currentState][a]);
            
            qTable[previousState][previousAction] += alpha * (reward + gamma * maxQ - qTable[previousState][previousAction]);
        }

        // Determine Action for this step
        vector<int> validActions = {0}; 
        if (portfolio.getCash() >= currentPrice) validActions.push_back(1);
        if (portfolio.getShares() > 0) validActions.push_back(2);
        
        int action = 0;
        bool forced = false;
        string reason = "AI";

        // RISK MANAGEMENT PROTOCOL (Stop-Loss and Take-Profit)
        if (portfolio.getShares() > 0) {
            double avgPrice = portfolio.getAveragePurchasePrice();
            if (currentPrice < avgPrice * 0.97) { action = 2; forced = true; reason = "Stop-Loss"; }
            else if (currentPrice > avgPrice * 1.05) { action = 2; forced = true; reason = "Take-Profit"; }
        }

        if (!forced) {
            double random_val = (double)rand() / RAND_MAX;
            if (random_val < currentEpsilon) {
                action = validActions[rand() % validActions.size()];
            } else {
                double bestQ = -1e9;
                for (int a : validActions) {
                    if (qTable[currentState][a] > bestQ) {
                        bestQ = qTable[currentState][a];
                        action = a;
                    }
                }
            }
        }

        // Execute action
        if (action == 1) {
            int sharesToBuy = portfolio.getCash() / currentPrice;
            if (portfolio.buy(currentPrice, sharesToBuy)) {
                if (!isTraining) tradeHistory.push_back({(int)i, "BUY", currentPrice, sharesToBuy, reason});
            }
        } else if (action == 2) {
            int sharesToSell = portfolio.getShares();
            if (portfolio.sell(currentPrice, sharesToSell)) {
                if (!isTraining) tradeHistory.push_back({(int)i, "SELL", currentPrice, sharesToSell, reason});
            }
        }

        // Prepare for next step
        if (isTraining) {
            previousState = currentState;
            previousAction = action;
            previousPortfolioValue = currentPortfolioValue;
        } else {
            previousState = -1; // disable updates
        }
    }
}



void TradingBot::printResults(const MarketData& marketData) const {
    const auto& data = marketData.getData();
    if (data.empty()) return;

    double currentPrice = data.back().price;
    double finalValue = portfolio.getTotalValue(currentPrice);

    cout << "\n================ SIMULATION RESULTS ================\n";
    cout << "Initial Balance:      $" << fixed << setprecision(2) << initialBalance << "\n";
    cout << "Final Portfolio Value:$" << finalValue << " (Cash: $" << portfolio.getCash() << ", Shares: " << portfolio.getShares() << ")\n";
    cout << "Net Profit/Loss:      $" << (finalValue - initialBalance) << "\n";
    cout << "Algorithm Return:     " << ((finalValue - initialBalance) / initialBalance) * 100.0 << "%\n";
    cout << "Total Trades Executed:" << tradeHistory.size() << "\n\n";

    double buyAndHoldReturn = ((currentPrice - data.front().price) / data.front().price) * 100.0;
    cout << "Benchmark (Buy and Hold Return): " << buyAndHoldReturn << "%\n\n";

    cout << "--- Q-LEARNING STATS ---\n";
    int visitedStates = 0;
    for (int i = 0; i < 96; i++) {
        if (qTable[i][0] != 0 || qTable[i][1] != 0 || qTable[i][2] != 0) visitedStates++;
    }
    cout << "States Explored: " << visitedStates << " / 96\n";
    cout << "Learning Rate (alpha): " << alpha << "\n";
    cout << "Discount Factor (gamma): " << gamma << "\n";
    cout << "Exploration Rate (epsilon): " << epsilon << "\n\n";

    cout << "--- LATEST TRADES ---\n";
    int start = max(0, (int)tradeHistory.size() - 5);
    for (size_t i = start; i < tradeHistory.size(); ++i) {
        cout << tradeHistory[i].type << " " << tradeHistory[i].shares 
                  << " shares at $" << tradeHistory[i].price << " (Day " << tradeHistory[i].dayIndex 
                  << ") [" << tradeHistory[i].reason << "]\n";
    }
    cout << "====================================================\n\n";
}
