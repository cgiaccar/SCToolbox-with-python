#!/bin/zsh

set -x

g++ -std=c++17 -Wall -Wpedantic -L/usr/local/muparserx-4.0.12 -lmuparserx src/main.cpp src/approximator.cpp src/midpoint_approximator.cpp src/trapezoidal_approximator.cpp src/simpson_approximator.cpp -o main

set +x

if [ $? -eq 0 ]; then
    echo "Build successful! You can run the program using ./main. If you run ./main demo, the app will showcase three illustrative examples."
else
    echo "Build failed."
fi
