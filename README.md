[README.md](https://github.com/user-attachments/files/26187858/README.md)
# Q-Learning Trading Bot Simulator 

An advanced algorithmic trading simulator written in pure C++ that uses Reinforcement Learning (Q-Learning) to autonomously learn how to trade stocks and maximize portfolio returns over historical data.

## Features 
- **Q-Learning Engine**: The bot explores the market using an Epsilon-Greedy policy and learns through the Temporal Difference (TD) algorithm to update a 96-state Q-table.
- **Dynamic Epoch Training**: Allows you to train the AI over thousands of epochs, with the exploration rate (`epsilon`) decaying logically over time to refine its strategy.
- **Technical Indicators**: The bot's state space is driven by robust market indicators calculated from raw CSV data:
  - Simple Moving Average (SMA) Trend
  - Historical Price Drops
  - Market Volatility
  - Relative Strength Index (RSI)
  - Moving Average Convergence Divergence (MACD)
- **Built-in Risk Management**: Bypasses AI logic for hardcoded Take-Profit (+5%) and Stop-Loss (-3%) mechanisms to securely lock profits and stop bleeding.
- **Zero Dependencies**: Pure C++17 Standard Template Library (STL). No bloated external libraries required.

## Build and Run 

The project structure is clean and ready to compile on any system. 

### Quick Start (Pre-compiled Windows)
For a quick test on Windows, jump straight in by executing the included binary:
```cmd
.\build\TradingBot.exe
```

### Manual Compilation
If you want to modify the source code or run on macOS/Linux, re-compile using `g++` or `CMake`:
```bash
# Using GCC directly
g++ src/*.cpp -o build/TradingBot.exe

# Or using CMake
mkdir build
cd build
cmake ..
cmake --build .
```

## How to use 
When you run the bot, you will enter the interactive simulation CLI:
1. Hit `1` to **Load Market Data**: Parses the historical `data/stock_data.csv`.
2. Hit `2` to **Train AI Model**: Select the number of epochs (e.g. `100` or `1000`). The bot will simulate trading over the data multiple times to build experience in its Q-table.
3. Hit `3` to **Run Test Simulation**: Unleash the trained model to interact with the market using its learned strategy (`epsilon = 0`).
4. Hit `4` to **View Results**: Displays net profit, total trades, states explored, algorithm return, and compares the bot against a standard Buy-and-Hold benchmark.
