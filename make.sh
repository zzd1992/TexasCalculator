#!/usr/bin/env bash

if [ ! -d "build" ]; then
  mkdir build
fi

cd src
g++ -std=c++11 -O3 -fPIC -shared \
api.cpp \
tables.cpp \
solver.cpp \
-o ../build/solver.so
