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
#include <memory>
#include <string>
#include <tuple>
#include <vector>

class Day3 {
  std::vector<std::vector<char>> board;
  std::map<std::tuple<int, int, int>, int> n_locations;
public:
  Day3() {
    loadInput("inputs/day3.txt");
  };
  ~Day3() {};

  void loadInput(const std::string& filename) {
    std::ifstream fin(filename);
    std::string line;
    while (std::getline(fin, line)) {
      std::vector<char> row;
      for (int j = 0; j < line.length(); j++) {
        char ch = line[j];
        std::cout << ch;
        row.push_back(ch);
      }
      std::cout << std::endl;
      board.push_back(row);
    }
  }

  void process() {}









  bool haveSymbolAbove1(int l_idx, int s_idx, int e_idx) {
    if (l_idx <= 0) {
       return false;
    } else {
        l_idx -= 1;
    }
    if(s_idx <= 0) {
      s_idx = 0;
    } else {
      s_idx -= 1;
    }
    if(e_idx < board[l_idx].size()) {
      e_idx += 1;
    }

    for (int i = s_idx ; i < e_idx; i++) {
      // check rules
      if (!std::isdigit(board[l_idx][i]) && board[l_idx][i] != '.') {
        return true;
      }
    }
    return false;
  }


  bool haveSymbolBefore1(int s_idx, std::shared_ptr<std::vector<char>> row) {
    if(s_idx <= 0) {
      s_idx = 0;
    } else {
      s_idx -= 1;
    }
    char left_c = row->at(s_idx);
    bool left = (!std::isdigit(left_c) && left_c != '.');
    
    return left;
  }

  void addValueToLocation1(int l_idx, int s_idx, int e_idx, int value, std::shared_ptr<std::vector<char>> row) {
    std::tuple<int, int, int> key = { l_idx, s_idx, e_idx };
    std::cout << l_idx << " " << s_idx \
        << " " << e_idx << " " << value << "\n"; 
    std::cout << haveSymbolAbove1(l_idx, s_idx, e_idx) << " | " <<  haveSymbolBefore1(s_idx, row) << std::endl;

    if(haveSymbolAbove1(l_idx, s_idx, e_idx) || haveSymbolBefore1(s_idx, row)) {
      if (!n_locations[key]) {
        n_locations[key] = value;
      }
    }
  }

  void loadInput1(const std::string& filename) {
    std::ifstream fin(filename);
    std::string line;
    int line_idx = 0;
    while (std::getline(fin, line)) {
      std::vector<char> row;
      bool isNumberTag = false;
      int number = 0;
      int startIdx = 0;
      int endIdx = 0;
      for (int j = 0; j < line.length(); j++) {
        char ch = line[j];
        //std::cout << ch;
        row.push_back(ch);
        if (std::isdigit(ch)) {
          if (number == 0) {
            startIdx = j;
          }
          isNumberTag = true;
          number = std::stoi(std::to_string(number) + ch);
          // is number
        } else {
          // is a symbol
          if (number > 0 ) {
            endIdx = j;
            addValueToLocation1(line_idx, startIdx, endIdx, number, std::make_shared<std::vector<char>>(row));
          }
          if(ch != '.') {
            
          }
          isNumberTag = false;
          number = 0;
        }
      }
      board.push_back(row);
      line_idx++;
    }
  }

  void printLocations () {
    for(auto location : n_locations) {
      std::cout << std::get<0>(location.first) << " " << std::get<1>(location.first) \
        << " " << std::get<2>(location.first) << " " << location.second << "\n"; 
    }
  }

  void test() {
    //printLocations();
  };
};
