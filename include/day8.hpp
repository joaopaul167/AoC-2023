#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

class Day8 {
  std::map<std::string, std::pair<std::string, std::string>> nodes;
  int steps = 0;
  std::string instructions;
  int index = 0;

  int getPath(std::string node) {
    if (node == "ZZZ") {
      return steps;
    }
    if (nodes.find(node) == nodes.end()) {
      return 0;
    }
    steps++;
    std::pair<std::string, std::string> nodePair = nodes[node];
    if (index % instructions.size() == 0) {
      index = 0;
    }
    if (instructions[index++] == 'L') {
      return getPath(nodePair.first);
    }
    return getPath(nodePair.second);
  }

  void solve() {
    long sum = getPath("AAA");
    std::cout << "Test Day 8 - Part 1: " << sum << std::endl;
  };

  void solve2() {
    std::map<std::string, std::pair<std::string, std::string>> entries;
    for (auto &entry : nodes) {
      if (entry.first[2] == 'A') {
        std::pair<std::string, std::string> nodePair = entry.second;
        entries[entry.first] = std::make_pair(nodePair.first, nodePair.second);
      }
    }
  }

  void parseInput(const std::string &filename) {
    std::ifstream fin(filename);
    std::string line;
    if (std::getline(fin, instructions)) {
      while (std::getline(fin, line)) {
        std::istringstream iss(line);
        std::string key, value;
        if (std::getline(iss, key, '=')) {
          if (std::getline(iss, value)) {
            key.erase(0, key.find_first_not_of(" \t\r\n"));
            key.erase(key.find_last_not_of(" \t\r\n") + 1);

            value.erase(1, value.find_first_not_of(" \t\r\n"));
            value.erase(value.find_last_not_of(" \t\r\n") + 1);

            value.erase(0, value.find_first_not_of(" \t\r\n"));
            value.erase(value.find_last_not_of(" \t\r\n"));

            // Extract values from the parentheses
            std::istringstream valuesStream(value);
            std::string value1, value2;
            if (std::getline(valuesStream, value1, ',') &&
                std::getline(valuesStream, value2, ',')) {
              value1.erase(0, value1.find_first_not_of(" \t\r\n"));
              value1.erase(value1.find_last_not_of(" \t\r\n") + 1);

              value2.erase(0, value2.find_first_not_of(" \t\r\n"));
              value2.erase(value2.find_last_not_of(" \t\r\n") + 1);

              nodes[key] = std::make_pair(value1, value2);
            }
          }
        }
      }
    }
  }

public:
  Day8() {
    parseInput("inputs/day8.txt");
    solve();
  };
};
