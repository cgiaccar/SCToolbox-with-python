#!/bin/zsh

set -x

g++ -std=c++17 -Wall -Wpedantic -I/usr/local/boost_1_83_0 src/main.cpp src/statistics.cpp -o main

set +x

if [ $? -eq 0 ]; then
    echo "Build successful! You can run the program using ./main [path/to/input/file] [path/to/output/file]"
else
    echo "Build failed."
fi
