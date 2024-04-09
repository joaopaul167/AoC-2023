#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <string>

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
    for (const auto &it : suit) {
      cardCount[it]++;
    }
    if (cardCount.find('J') != cardCount.end()) {
      j_count = cardCount['J'];
    }
    rank = calculateRank();
  };

  bool operator<(const Hand &other) const {
    if (rank == other.rank) {
      return mostValuableHand(suit, other.suit);
    }
    return rank < other.rank;
  }

  bool mostValuableHand(const std::string &m_suit,
                        const std::string &o_suit) const {
    if (m_suit.size() == 0)
      return false;
    if (o_suit.size() == 0)
      return true;
    if (cards.at(m_suit.substr(0, 1)) == cards.at(o_suit.substr(0, 1))) {
      return mostValuableHand(m_suit.substr(1), o_suit.substr(1));
    }
    return cards.at(m_suit.substr(0, 1)) < cards.at(o_suit.substr(0, 1));
  }

  bool checkCount(int target) const {
    if (j_count == 5) {
      return true;
    }
    // use 1 for two pair, j is zero by design: protected by all assertions
    if (target == 1) {
      int count = 0;
      for (const auto &[card, value] : cardCount) {
        if (value == 2)
          count++;
      }
      return count == 2;
    }
    for (const auto &[card, value] : cardCount) {
      if (card == 'J')
        continue;
      if (value + j_count == target) {
        return true;
      }
    }
    return false;
  }

  bool checkFullHouse() const {
    int remainingJokers = j_count;

    bool hasPair = false;
    bool hasThreeOfAKind = false;

    for (const auto &[card, count] : cardCount) {
      if (card == 'J')
        continue;
      if (remainingJokers == 2) {
        if (count + 1 == 2) {
          hasPair = true;
          remainingJokers -= 1;
        } else if (count + 1 == 3) {
          hasThreeOfAKind = true;
          remainingJokers -= 1;
        } else if (count + 2 == 3) {
          hasThreeOfAKind = true;
          remainingJokers -= 2;
        }
      }
      if (count + remainingJokers == 2) {
        hasPair = true;
        if (remainingJokers > 0) {
          remainingJokers -= 1;
        }
      } else if (count + remainingJokers == 3) {
        hasThreeOfAKind = true;
        if (remainingJokers > 0) {
          remainingJokers -= 1;
        }
      }
    }
    return hasPair && hasThreeOfAKind;
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
    long sum = std::accumulate(
        hands.begin(), hands.end(), 0, [this](long sum, const Hand &h) {
          auto iter = hands.find(h);
          if (iter != hands.end()) {
            size_t currentIndex = std::distance(hands.begin(), iter);
            sum += h.bid * currentIndex;
          }
          return sum;
        });
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
