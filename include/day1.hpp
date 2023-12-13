// The newly-improved calibration document consists of lines of text;
// each line originally contained a specific calibration value that the Elves
// now need to recover. On each line, the calibration value can be found by
// combining the first digit and the last digit (in that order) to form a
// single two-digit number.

// Consider your entire calibration document (inputs/day1.txt).
// What is the sum of all of the calibration values?
// Part2-  Your calculation isn't quite right. It looks like some of the
// digits are actually spelled out with letters:
// one, two, three, four, five, six, seven, eight, and nine also count
// as valid "digits".
#pragma once
#include <cctype>
#include <ctime>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

class Day1 {
  int acc = 0;
  clock_t begin_t;
  std::vector<std::pair<int, int>> vector;

public:
  Day1() {
    begin_t = std::clock();
    loadInput("inputs/day1.txt");
  };
  ~Day1(){};

  void insertValue(std::pair<int, int> &pair, int value) {
    if (pair.first == 0) {
      pair.first = value;
      pair.second = value;
    } else {
      pair.second = value;
    }
  };

  void loadInput(const std::string &filename) {
    std::ifstream fin(filename);
    std::string line;
    while (fin >> line) {
      acc++;
      std::pair<int, int> pair(0, 0);
      for (int i = 0; i < line.length(); i++) {
        acc++;
        if (std::isdigit(line[i])) {
          insertValue(pair, int(line[i]) - 48);
        }
        if (line.substr(i, 3) == "one") {
          insertValue(pair, 1);
        }
        if (line.substr(i, 3) == "two") {
          insertValue(pair, 2);
        }
        if (line.substr(i, 5) == "three") {
          insertValue(pair, 3);
        }
        if (line.substr(i, 4) == "four") {
          insertValue(pair, 4);
        }
        if (line.substr(i, 4) == "five") {
          insertValue(pair, 5);
        }
        if (line.substr(i, 3) == "six") {
          insertValue(pair, 6);
        }
        if (line.substr(i, 5) == "seven") {
          insertValue(pair, 7);
        }
        if (line.substr(i, 5) == "eight") {
          insertValue(pair, 8);
        }
        if (line.substr(i, 4) == "nine") {
          insertValue(pair, 9);
        }
      }
      vector.push_back(pair);
    }
  }

  int sumAllValues() {
    int sum = 0;
    for (auto pair : vector) {
      acc++;
      std::string s1 = std::to_string(pair.first);
      std::string s2 = std::to_string(pair.second);
      sum += std::stoi(s1 + s2);
    }
    return sum;
  }

  void test() {
    std::cout << "Test Day 1: Part 2 sum: " << sumAllValues() << std::endl;
    std::cout << "Acc = " << acc << std::endl;
    std::cout << "Compute time: "
              << float(begin_t - std::clock()) / CLOCKS_PER_SEC << std::endl;
  }
};
