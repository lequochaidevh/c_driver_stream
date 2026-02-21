#!/usr/bin/bash

mkdir build/

g++ -std=c++17 use_typetrait.cpp -o ./build/execute

./build/execute

rm -rf build/