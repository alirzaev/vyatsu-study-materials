#!/bin/bash

g++ -std=c++14 -O2 main_master.cpp -o main_master -lpvm3
g++ -std=c++14 -O2 main_slave.cpp -o main_slave -lpvm3
