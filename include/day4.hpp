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
//
// Part 2
// There's no such thing as "points". Instead, scratchcards only cause you to
// win more scratchcards equal to the number of winning numbers you have.

// Specifically, you win copies of the scratchcards below the winning card equal
// to the number of matches. So, if card 10 were to have 5 matching numbers,
// you would win one copy each of cards 11, 12, 13, 14, and 15.

// Copies of scratchcards are scored like normal scratchcards and have the same
// card number as the card they copied. So, if you win a copy of card 10 and it
// has 5 matching numbers, it would then win a copy of the same cards that the
// original card 10 won: cards 11, 12, 13, 14, and 15. This process repeats
// until none of the copies cause you to win any more cards. (Cards will never
// make you copy a card past the end of the table.)

#include <cctype>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>

class Day4 {
  clock_t begin_t;
  int acc = 0;
  std::map<int, std::set<int>> win_map;
  std::map<int, std::set<int>> hand_map;
  std::map<int, int> count_map;
  std::map<int, int> win_matches;
  std::map<int, bool> processed_map;
  int sum = 0;

public:
  Day4() {
    begin_t = std::clock();
    loadInput("inputs/day4.txt");
  };
  ~Day4(){};

  void addWinSide(int idx, int value) { win_map[idx].insert(value); }

  void addHandside(int idx, int value) { hand_map[idx].insert(value); }

  int countOccurrences(const std::set<int> &set1, const std::set<int> &set2,
                       int idx) {
    if (count_map[idx])
      return count_map[idx];
    for (const auto &el : set2) {
      if (set1.find(el) != set1.end()) {
        count_map[idx]++;
      }
    }
    return count_map[idx];
  }

  void loadInput(const std::string &filename) {
    std::ifstream fin(filename);
    std::string line;
    int gameIdx = 1;
    while (std::getline(fin, line)) {
      bool inWinSide = true;
      int digit = 0;
      // position of ':' char
      for (int i = 8; i <= line.size() + 1; i++) {
        char ch = line[i];
        if (ch == '|') {
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

        if (i == line.size()) {
          addHandside(gameIdx, digit);
        }
      }
      countOccurrences(win_map[gameIdx], hand_map[gameIdx], gameIdx);
      gameIdx++;
    }
  }

  int valueDoubleNTimes(int number, int n) {
    for (int i = 0; i < n; i++) {
      number *= 2;
    }
    return number;
  }

  void process1() {
    int sum1 = 0;
    int idx = 1;
    while (win_map.size() > idx) {
      int count = count_map[idx];
      if (count > 1) {
        sum1 += valueDoubleNTimes(1, count - 1);
      } else if (count == 1) {
        sum1++;
      }
      idx++;
    }
    std::cout << "Test Day 4 - Part 1 sum: " << sum1 << std::endl;
  };

  int getMatches(int idx) {
    if (processed_map[idx]) {
      return win_matches[idx];
    }
    acc++;
    win_matches[idx]++;
    for (int i = idx + 1; i <= idx + count_map[idx]; i++) {
      win_matches[idx] += getMatches(i);
    }
    processed_map[idx] = true;
    return win_matches[idx];
  }

  void process2() {
    for (auto rit = win_map.rbegin(); rit != win_map.rend(); rit++) {
      getMatches(rit->first);
    }
    for (auto el : win_matches) {
      sum += el.second;
    }
    std::cout << "Test Day 4 - Part 2 sum: " << sum << std::endl;
  }

  void test() {
    process1();
    process2();
    std::cout << "Acc = " << acc << std::endl;
    std::cout << "Compute time: "
              << float(begin_t - std::clock()) / CLOCKS_PER_SEC << std::endl;
  };
};
