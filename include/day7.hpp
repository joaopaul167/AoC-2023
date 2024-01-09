#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <set>

const std::map<std::string, int> cards{
    {"A", 14}, {"K", 13}, {"Q", 12}, {"T", 10}, {"9", 9}, {"8", 8}, {"7", 7},
    {"6", 6},  {"5", 5},  {"4", 4},  {"3", 3},  {"2", 2}, {"J", 1},
};

struct Hand {
  const int bid;
  int j_count = 0;
  const std::string suit;
  int rank;
  std::map<char, int> cardCount;

  Hand(std::string s, int b) : suit{s}, bid{b} {
    for (auto it = suit.begin(); it != suit.end(); it++) {
      cardCount[*it]++;
    }
    if (cardCount.find('J') != cardCount.end()) {
      j_count = cardCount['J'];
    }
    rank = calculateRank();
  };

  void print() const {
    std::cout << suit << " " << bid << " " << rank << " "<< j_count << std::endl;
  }

  bool operator<(const Hand &other) const {
    if (rank == other.rank) {
      return mostValuableHand(suit, other.suit);
    }
    return rank < other.rank;
  }

  bool mostValuableHand(const std::string &ms, const std::string &os) const {
    if (ms.size() == 0)
      return false;
    if (os.size() == 0)
      return true;
    if (cards.at(ms.substr(0, 1)) == cards.at(os.substr(0, 1))) {
      return mostValuableHand(ms.substr(1), os.substr(1));
    }
    return cards.at(ms.substr(0, 1)) < cards.at(os.substr(0, 1));
  }

  bool checkCount(int target) const {
    if (j_count == 5) {
      return true;
    }
    // use 1 for two pair, j is zero by design: protected by all assertions
    if (target == 1) {
      int count = 0;
      for (auto it = cardCount.begin(); it != cardCount.end(); it++) {
        if (it->second == 2)
          count++;
      }
      return count == 2;
    }
    for (auto it = cardCount.begin(); it != cardCount.end(); it++) {
      if (it->first == 'J')
        continue;
      if (it->second + j_count == target) {
        return true;
      }
    }
    return false;
  }

  bool checkFullHouse() const {
    int j = j_count;
    bool pair = false;
    bool three = false;
    
    for (auto it = cardCount.begin(); it != cardCount.end(); it++) {
      if (it->first == 'J')
        continue;
      if (j == 2) {
        if (it->second + 1 == 2) {
          pair = true;
          j--;
          continue;
        } else if (it->second + 1 == 3) {
          three = true;
          j--;
          continue;
        } else if (it->second + 2 == 3) {
          three = true;
          j--;
          j--;
          continue;
        }
      }
      
      if (it->second + j == 2) {
        pair = true;
        if (j > 0) {
          j--;
        }
      } else if (it->second + j == 3) {
        three = true;
        if (j > 0) {
          j--;
        }
      }
    }
    return pair && three;
  }

  const int calculateRank() const {
    if (checkCount(5)) {
      return 7;
    } else if (checkCount(4)) {
      return 6;
    } else if (checkFullHouse()) {
      return 5;
    } else if (checkCount(3)) {
      return 4;
    } else if (checkCount(1)) {
      return 3;
    } else if (checkCount(2)) {
      return 2;
    }
    return 1;
  }
};

class Day7 {
public:
  std::set<Hand> hands;
  Day7() {
    parseInput("inputs/day7.txt");
    solve();
  };

  void solve() {
    int rank = 1;
    long sum = 0;
    for (auto it = hands.begin(); it != hands.end(); it++) {
      int final_bid = it->bid * rank;
      sum += final_bid;
      rank++;
    }

    std::cout << "Test Day 7 - Part 2: " << sum << std::endl;
  };

  void parseInput(const std::string &filename) {
    std::ifstream fin(filename);
    std::string line;
    while (std::getline(fin, line)) {
      std::istringstream iss(line);
      std::string suite;
      int bid;
      iss >> suite >> bid;
      Hand h(suite, bid);
      hands.insert(h);
    }
  }
};
