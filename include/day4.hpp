// The Elf leads you over to the pile of colorful cards. There, you discover 
// dozens of scratchcards, all with their opaque covering already scratched 
// off. Picking one up, it looks like each card has two lists of numbers 
// separated by a vertical bar (|): a list of winning numbers and then a 
// list of numbers you have. You organize the information into a table 
// (your puzzle input).

// As far as the Elf has been able to figure out, you have to figure out 
// which of the numbers you have appear in the list of winning numbers. 
// The first match makes the card worth one point and each match after 
// the first doubles the point value of that card.

#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>

class Day4 {
  std::map<int, std::set<int>> win_map;
  std::map<int, std::set<int>> hand_map;
  public: 
    Day4() {
      loadInput("inputs/day4.txt");
    };
    ~Day4() {};
  
    void addWinSide(int idx, int value) {
      win_map[idx].insert(value);
    }

    void addHandside(int idx, int value) {
      hand_map[idx].insert(value);
    }

    void loadInput(const std::string& filename) {
      std::ifstream fin(filename);
      std::string line;
      int gameIdx = 1;
      while (std::getline(fin, line)) {
        bool inWinSide = true;
        int digit = 0;
        // position of ':' char 
        for (int i = 8; i <= line.size() + 1; i++) {
          char ch = line[i];         
          if(ch == '|') {
            inWinSide = false;
            continue;
          }
          if (ch == ' ' && digit > 0) {
            if (inWinSide) {
              addWinSide(gameIdx, digit);
            } else {
              addHandside(gameIdx, digit);
            }
            digit = 0;
          }
          if (std::isdigit(ch)) {
            std::string s_digit = std::to_string(digit); 
            digit = std::stoi(s_digit + ch);
          }
            
          if ( i == line.size()){
            addHandside(gameIdx, digit);
          }
        }
        gameIdx++;
      }
    }

    void print() {
      int idx = 1;
      while(win_map[idx].size() > 0) {
        std::cout << "Game : " << idx << " winning: ";
        for (auto win : win_map[idx]) {
          std::cout << win << " ";
        }
        std::cout << " |  ";
        for (auto hand: hand_map[idx]) {
          std::cout << hand << " ";
        }
        std::cout << std::endl;
        idx++; 
      }
    }

    int valueDoubleNTimes(int number, int n) {
      for (int i = 0; i < n ; i++){
        number *= 2;
      }
      return number;
    }

    int countOccurrences(const std::set<int>& set1, const std::set<int>& set2) {
      int count = 0;

      for (const auto& el : set2) {
        // if found return the pointer to the el, if not return a pointer to the
        // end, thats why we check the end
        if (set1.find(el) != set1.end()) {
          count++;
        }
      }
      return count;
    } 

    void process() {
      int sum = 0;
      int idx = 1;
      while(win_map.size() > idx) {
        int count = countOccurrences(win_map[idx], hand_map[idx]); 
        if (count > 1 ) {
          sum +=  valueDoubleNTimes(1, count - 1);
        } else if (count == 1) {
          sum++;
        } 
        idx++;
      }
      std::cout << "Test Day 4 - Part 1 sum: " << sum << std::endl;
    };
    void test(){
      process();
    };
};
