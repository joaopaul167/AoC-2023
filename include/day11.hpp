#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
template <typename T> using matrix = std::vector<std::vector<T>>;
typedef std::pair<int, int> boardLongCoord;

class Day11 {
  matrix<char> universe;
  std::map<std::pair<boardLongCoord, boardLongCoord>, int> distances;

public:
  Day11() {
    parseInput("inputs/day11.txt");
    solve();
    //solve2();
  };

  matrix<char> transpose(matrix<char> &u) {
    matrix<char> temp;
    for (int i = 0; i < u[0].size(); i++) {
      std::vector<char> row;
      for (int j = 0; j < u.size(); j++) {
        row.push_back(u[j][i]);
      }
      temp.push_back(row);
    }
    return temp;
  }

  std::vector<int> emptyRows(matrix<char> &u) {
    std::vector<int> emptyRows;
    std::vector<char> emptyRow(u[0].size(), '.');
    for (int i = 0; i < u.size(); i++) {
      if (u[i] == emptyRow) {
        emptyRows.push_back(i);
      }
    }
    return emptyRows;
  };

  matrix<char> expandUniverse() {
    int count = 0;
    for (auto &i : emptyRows(universe)) {
      universe.insert(universe.begin() + i + count,
                      std::vector<char>(universe[0].size(), '.'));
      count++;
    }
    matrix<char> transUniverse = transpose(universe);
    count = 0;
    for (auto &i : emptyRows(transUniverse)) {
      transUniverse.insert(transUniverse.begin() + i + count,
                           std::vector<char>(transUniverse[0].size(), '.'));
      count++;
    }
    matrix<char> f_universe = transpose(transUniverse);
    return f_universe;
  };

  int traverse(const boardLongCoord &start, const boardLongCoord &end) {
    int distance = 0;
    boardLongCoord current = start;
    while (current != end) {
      moveGalaxy(current, end);
      distance++;
    }
    return distance;
  }

  void solve() {
    matrix<char> expandedUniverse = expandUniverse();
    std::vector<boardLongCoord> galaxies;
    for (int i = 0; i < expandedUniverse.size(); i++) {
      for (int j = 0; j < expandedUniverse[0].size(); j++) {
        if (expandedUniverse[i][j] == '#') {
          galaxies.push_back(std::make_pair(i, j));
        }
      }
    }
    std::set<std::pair<boardLongCoord, boardLongCoord>> pairs;
    for (auto &i : galaxies) {
      for (auto &j : galaxies) {
        if (i != j) {
          if (pairs.find(std::make_pair(j, i)) != pairs.end()) {
            continue;
          }
          if (pairs.find(std::make_pair(i, j)) != pairs.end()) {
            continue;
          }
          pairs.insert(std::make_pair(i, j));
        }
      }
    }
    for (auto &i : pairs) {
      int distance = traverse(i.first, i.second);
      distances[i] = distance;
    }
    int sum = 0;
    for (auto &i : distances) {
      sum += i.second;
    }
    std::cout << "Test Day 11 - Part 1: " << sum << std::endl;
  };

  void moveGalaxy(boardLongCoord &galaxy, const boardLongCoord &end) {
    if (galaxy.first < end.first) {
      galaxy.first++;
    } else if (galaxy.first > end.first) {
      galaxy.first--;
    } else if (galaxy.second < end.second) {
      galaxy.second++;
    } else if (galaxy.second > end.second) {
      galaxy.second--;
    }
  }

  int traverseWithHyperSpace(const boardLongCoord &start,
                             const boardLongCoord &end,
                             std::vector<int> &rowsIdxToExpand,
                             std::vector<int> &colsIdxToExpand) {
    int distance = 0;
    boardLongCoord current = start;
    std::set<int> alreadyChecked;
    while (current != end) {
      moveGalaxy(current, end);
      distance++;
      int hyperSpace = 10;
      for (auto &i : rowsIdxToExpand) {
        if (current.first == i &&
            alreadyChecked.find(i) == alreadyChecked.end()) {
          std::cout << "Expanding row " << i << std::endl;
          std::cout << "Distance: " << distance << std::endl;
          distance += hyperSpace;
          distance--;
          std::cout << "Distance after expansion: " << distance << std::endl;
          alreadyChecked.insert(i);
        }
      }
      for (auto &i : colsIdxToExpand) {
        if (current.second == i &&
            alreadyChecked.find(i) == alreadyChecked.end()) {
          std::cout << "Expanding col " << i << std::endl;
          std::cout << "Distance: " << distance << std::endl;
          distance += hyperSpace;
          distance--;
          std::cout << "Distance after expansion: " << distance << std::endl;
          alreadyChecked.insert(i);
        }
      }
    }
    return distance;
  }

  std::pair<std::vector<int>, std::vector<int>> getWhereToExpandUniverse() {
    auto rows = emptyRows(universe);
    matrix<char> transUniverse = transpose(universe);
    auto cols = emptyRows(transUniverse);
    return std::make_pair(rows, cols);
  };

  void solve2() {
    auto rowsAndCols = getWhereToExpandUniverse();
    auto rowsIdxToExpand = rowsAndCols.first;
    auto colsIdxToExpand = rowsAndCols.second;
    std::vector<boardLongCoord> galaxies;
    for (int i = 0; i < universe.size(); i++) {
      for (int j = 0; j < universe[0].size(); j++) {
        if (universe[i][j] == '#') {
          galaxies.push_back(std::make_pair(i, j));
        }
      }
    }
    std::set<std::pair<boardLongCoord, boardLongCoord>> pairs;
    for (auto &i : galaxies) {
      for (auto &j : galaxies) {
        if (i != j) {
          if (pairs.find(std::make_pair(j, i)) != pairs.end()) {
            continue;
          }
          if (pairs.find(std::make_pair(i, j)) != pairs.end()) {
            continue;
          }
          pairs.insert(std::make_pair(i, j));
        }
      }
    }
    for (auto &i : pairs) {
      int distance = traverseWithHyperSpace(i.first, i.second, rowsIdxToExpand,
                                            colsIdxToExpand);
      distances[i] = distance;
    }
    int sum = 0;
    for (auto &i : distances) {
      sum += i.second;
    }
    std::cout << "Test Day 11 - Part 2: " << sum << std::endl;
  }
  void parseInput(const std::string &filename) {
    std::ifstream fin(filename);
    std::string line;
    while (std::getline(fin, line)) {
      std::istringstream iss(line);
      std::vector<char> row;
      char c;
      while (iss >> c) {
        row.push_back(c);
      }
      universe.push_back(row);
    }
    fin.close();
  }
};
