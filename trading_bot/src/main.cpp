#include "MarketData.h"
#include "TradingBot.h"
#include <iostream>
#include <string>

using namespace std;

void displayMenu() {
    cout << "\n=== ALGORITHMIC TRADING BOT SIMULATOR ===\n";
    cout << "1. Load Market Data (CSV)\n";
    cout << "2. Train AI Model (Epochs)\n";
    cout << "3. Run Test Simulation\n";
    cout << "4. Print Performance Results\n";
    cout << "5. Exit\n";
    cout << "Select an option: ";
}

int main() {
    // Hardcoded initial parameters
    double initialBalance = 10000.0;
    int shortSMA = 5;
    int longSMA = 20;

    MarketData data("data/stock_data.csv");
    TradingBot bot(initialBalance, shortSMA, longSMA);
    
    bool dataLoaded = false;
    bool modelTrained = false;
    bool simulationRun = false;

    int choice = 0;
    while (true) {
        displayMenu();
        if (!(cin >> choice)) {
            if (cin.eof()) {
                cout << "\n[SYSTEM] End of input recognized. Exiting simulator gracefully.\n";
                break;
            }
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) {
            case 1:
                cout << "[SYSTEM] Loading data from data/stock_data.csv...\n";
                if (data.loadData()) {
                    cout << "[SUCCESS] Loaded " << data.getData().size() << " daily records.\n";
                    dataLoaded = true;
                } else {
                    cout << "[FAILED] Could not load data. Ensure data/stock_data.csv exists.\n";
                }
                break;
            case 2:
                if (!dataLoaded) {
                    cout << "[ERROR] Please load data before training!\n";
                } else {
                    int epochs = 1000;
                    cout << "Enter number of epochs for training (e.g., 1000): ";
                    if (!(cin >> epochs)) { 
                        cin.clear(); 
                        cin.ignore(10000, '\n'); 
                        epochs = 1000; 
                    }
                    bot.train(data, epochs);
                    modelTrained = true;
                }
                break;
            case 3:
                 if (!dataLoaded) {
                     cout << "[ERROR] Please load data first!\n";
                 } else {
                     if (!modelTrained) {
                         cout << "[WARN] Model not trained! Running with empty Q-table (exploring blindly).\n";
                     }
                     bot.test(data);
                     simulationRun = true;
                 }
                 break;
            case 4:
                 if (!simulationRun) {
                     cout << "[ERROR] No testing results available. Run test simulation first!\n";
                 } else {
                     bot.printResults(data);
                 }
                 break;
            case 5:
                cout << "Exiting system. Goodbye!\n";
                return 0;
            default:
                cout << "[ERROR] Invalid choice. Select 1-5.\n";
        }
    }

    return 0;
}
