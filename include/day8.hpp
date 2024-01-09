#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
class Day8 {
public:
  Day8() {
    parseInput("inputs/day8.txt");
    solve();
  };

  void solve() {
    int rank = 1;
    long sum = 0;

    std::cout << "Test Day 8 - Part 2: " << sum << std::endl;
  };

  void parseInput(const std::string &filename) {
    std::ifstream fin(filename);
    std::string line;
    while (std::getline(fin, line)) {
      std::istringstream iss(line);
      std::string suite;
      int bid;
      iss >> suite >> bid;
    }
  }
};
