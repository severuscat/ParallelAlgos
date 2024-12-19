#!/bin/bash
export PARLAY_NUM_THREADS=4
g++ -I./parlay -std=c++17 -pthread main.cpp -o bfs && ./bfs 
