// The engine schematic (your puzzle input) consists of a visual representation 
// of the engine. There are lots of numbers and symbols you don't really understand, 
// but apparently any number adjacent to a symbol, even diagonally, is a "part 
// number" and should be included in your sum. (Periods (.) do not count as a symbol.)

// What is the sum of all of the part numbers in the engine schematic?
//
// My first idea is to store all the numbers locations, than iterate over this 
// locations to test if exist an symbol around, if exist sum it.

#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

class Day3 {
  std::map<std::pair<int, int>, int> n_map;
  std::map<std::pair<int, int>, char> s_map;
public:
  Day3() {
    loadInput("inputs/day3.txt");
  };
  ~Day3() {};

  void addValue (int xs, int ys, int value) {
    n_map[{xs, ys}] = value;
  }

  void addSymbol(int xs, int ys, char symbol) {
    s_map[{xs, ys}] = symbol;
  }

  bool haveSymbolAround(int xs, int ys, int n_len) {
    if (s_map[{xs, ys - 1}] || s_map[{xs, ys + n_len}]) return true;
    for (int i = ys - 1; i <= ys + n_len; i++) {
      if (s_map[{xs - 1, i}] || s_map[{xs + 1, i}]) return true;
    }
    return false;
  }

  void loadInput(const std::string& filename) {
    std::ifstream fin(filename);
    std::string line;
    int line_idx = 0;
    while (std::getline(fin, line)) {
      int number = 0;
      for (int j = 0; j < line.length(); j++) {
        char ch = line[j];
        if (std::isdigit(ch)) {
          number = std::stoi(std::to_string(number) + ch);
        } else {
          if (number > 0 ) {
            addValue(line_idx, j - std::to_string(number).length(), number);
            number = 0;
          }
          if (ch != '.') {
            addSymbol(line_idx, j, ch);
          }
        }
      }
      line_idx++;
    }
  }

  void process() {
    int sum = 0;
    for (auto value: n_map) {
      auto key = value.first;
      int number = value.second;
      if(haveSymbolAround(key.first, key.second, std::to_string(number).length())) {
        sum += number;
      }
    }
    std::cout << sum << std::endl;
  }

  void test() {
    process();
  };
};
