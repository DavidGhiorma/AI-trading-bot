cmake_minimum_required(VERSION 3.10)
project(TradingBotSimulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add header files directory
include_directories(src)

# Find all source files
file(GLOB SOURCES "src/*.cpp")

# Create executable
add_executable(TradingBot ${SOURCES})
