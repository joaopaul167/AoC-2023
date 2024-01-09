#!/bin/zsh
clear
echo "start building Advent of Code y(2023)"

INCLUDE_DIR=include/*.hpp

output=$(\
  g++ main.cpp \
  -o test.out \
  -I $INCLUDE_DIR \
  -std=c++20 \
  )

echo "finish building"
./test.out
echo "finished"

