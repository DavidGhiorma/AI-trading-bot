#pragma once

class Portfolio {
public:
    Portfolio(double initial_balance);
    
    bool buy(double price, int amount);
    bool sell(double price, int amount);
    
    double getCash() const;
    int getShares() const;
    double getTotalValue(double current_price) const;
    double getAveragePurchasePrice() const;

private:
    double cash;
    int shares;
    double averagePurchasePrice = 0.0;
};
