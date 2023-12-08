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
#include <numeric>
#include <string>
#include <vector>

enum PositionType {
  PART_NUMBER,
  NUMBER,
  SYMBOL,
  NOTHING
};

class Day3 {
  int acc = 0;
  std::map<std::pair<int, int>, int> n_map;
  std::map<std::pair<int, int>, char> s_map;
  std::vector<std::vector<std::pair<char, PositionType>>> grid;
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
      acc++;
      if (s_map[{xs - 1, i}] || s_map[{xs + 1, i}]) return true;
    }
    return false;
  }
  
  int findCompletePartNumber( int sx, int sy) {
    std::string partNumber_str = "";
    partNumber_str += grid[sx][sy].first;
    for(int col = (sy + 1); col < grid.size(); col++) {
      if(grid[sx][col].second == PositionType::NUMBER) {
        partNumber_str += grid[sx][col].first;
        grid[sx][col].second = PositionType::PART_NUMBER;
      } else break;
    }
    for(int col = (sy - 1); col >= 0; col--) {
      if(grid[sx][col].second == PositionType::NUMBER) {
        partNumber_str = grid[sx][col].first + partNumber_str;
        grid[sx][col].second = PositionType::PART_NUMBER;
      } else break;
    }
    return std::stoi(partNumber_str);
  }

  std::vector<std::pair<int,int>> getAdjacentNumbersPos(int sx, int sy) {
    std::vector<std::pair<int, int>> adjCoor = {};
    //North
    if (sx > 0) {
      if (grid[sx - 1][sy].second == PositionType::NUMBER) {
        adjCoor.push_back({sx - 1, sy});
      }
    }     
    //Northeast
    if( (sx > 0) && (sy < grid[sx].size()) ) {
      if(grid[sx - 1][sy + 1].second == PositionType::NUMBER) {
        adjCoor.push_back({sx - 1, sy + 1});
      }
    }
    //East
    if( sy < grid[sx].size() ) {
      if(grid[sx][sy + 1].second == PositionType::NUMBER) {
        adjCoor.push_back({sx, sy + 1});
      }
    }
    //Southeast
    if( (sx > 0) && (sy < grid[sx].size()) ) {
      if( grid[sx + 1][sy + 1].second == PositionType::NUMBER){
        adjCoor.push_back({sx + 1, sy + 1});
      }
    }
    //South
    if( sx > 0 ) {
      if(grid[sx + 1][sy].second == PositionType::NUMBER) {
        adjCoor.push_back({sx + 1, sy});
      }
    }
    //Southwest
    if( (sx > 0 ) && (sy > 0) ) {
      if( grid[sx + 1][sy - 1].second == PositionType::NUMBER ){
        adjCoor.push_back({sx + 1, sy -1});
      }
    }
    //West
    if( sy > 0 ) {
      if( grid[sx][sy - 1].second == PositionType::NUMBER) {
        adjCoor.push_back({sx, sy - 1});
      }
    }
    //Northwest
    if( (sx > 0) && (sy > 0) ) {
      if( grid[sx - 1][sy - 1].second == PositionType::NUMBER) {
        adjCoor.push_back({sx - 1, sy - 1});
      }
    }

    return adjCoor;
  }

  void loadInput(const std::string& filename) {
    std::ifstream fin(filename);
    std::string line;
    int line_idx = 0;
    while (std::getline(fin, line)) {
      acc++;
      int number = 0;
      std::vector<std::pair<char, PositionType>> row;
      for (int j = 0; j <= line.length(); j++) {
        acc++;
        char ch = line[j];
        if (std::isdigit(ch)) {
          row.push_back({ch, PositionType::NUMBER});
          number = std::stoi(std::to_string(number) + ch);
        } else {
          row.push_back({ch, PositionType::NOTHING});
          if (number > 0 ) {
            addValue(line_idx, j - std::to_string(number).length(), number);
            number = 0;
          }
          if (ch != '.') {
            addSymbol(line_idx, j, ch);
          }
        }
      }
      grid.push_back(row);
      line_idx++;
    }
  }

  void process() {
    int sum = 0;
    int sumGear = 0;
    for (auto value: n_map) {
      acc++;
      auto key = value.first;
      int number = value.second;
      if(haveSymbolAround(key.first, key.second, std::to_string(number).length())) {
        sum += number;
      }
    }
    std::cout << "Test Day 3: Part 1 sum: "<< sum << std::endl;
    std::cout << "Acc = " << acc << std::endl;

    for(auto symbol: s_map) {
      if (symbol.second != '*') continue;
      auto key = symbol.first;
      std::vector<int> adjNumbers;
      auto adjNumbersPos = getAdjacentNumbersPos(key.first, key.second);
      for (auto adjNum : adjNumbersPos) {
        if (grid[adjNum.first][adjNum.second].second == PositionType::NUMBER) {
          int completeNumber = findCompletePartNumber(adjNum.first, adjNum.second);
          adjNumbers.push_back(completeNumber);
        }
      }
      if(adjNumbers.size() == 2) {
        sumGear += adjNumbers[0] * adjNumbers[1];
      }
      adjNumbers.clear();
    }
    std::cout << "Test Day 3: Part 2 sum: "<< sumGear << std::endl;
  }

  void test() {
    process();
  };
};
