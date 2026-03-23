#include "Portfolio.h"

using namespace std;

Portfolio::Portfolio(double initial_balance) : cash(initial_balance), shares(0) {}

bool Portfolio::buy(double price, int amount) {
    if (amount <= 0 || cash < price * amount) return false;
    
    // Average out the purchase price
    double totalCost = averagePurchasePrice * shares + price * amount;
    
    cash -= price * amount;
    shares += amount;
    
    averagePurchasePrice = totalCost / shares;
    return true;
}

bool Portfolio::sell(double price, int amount) {
    if (amount <= 0 || shares < amount) return false;
    cash += price * amount;
    shares -= amount;
    
    if (shares == 0) averagePurchasePrice = 0.0; // Reset when empty
    return true;
}

double Portfolio::getCash() const { return cash; }
int Portfolio::getShares() const { return shares; }
double Portfolio::getAveragePurchasePrice() const { return averagePurchasePrice; }

double Portfolio::getTotalValue(double current_price) const {
    return cash + (shares * current_price);
}
