#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

template <typename T> using matrix = std::vector<std::vector<T>>;
enum class Pipe { VERT, HORIZ, L, J, SEVEN, F, GROUND, START };
enum class Direction { NORTH, SOUTH, EAST, WEST };
typedef std::pair<int, int> boardCoord;

class Day10 {
public:
  matrix<char> board;
  boardCoord start;
  std::map<int, std::vector<boardCoord>> paths;
  int path_idx;

  int farthest = 0;
  Day10() {
    parseInput("inputs/day10.txt");
    solve();
  };

  void getNext(boardCoord coord, Direction dir, int path) {
    boardCoord newCoord = getNewCoord(coord, dir);
    if (newCoord.first < 0 || newCoord.second < 0) {
      paths.erase(path);
      return;
    }
    auto boardPiece = board[newCoord.first][newCoord.second];
    paths[path].push_back(newCoord);
    if (boardPiece == '|') {
      if (dir == Direction::NORTH) {
        getNext(newCoord, Direction::NORTH, path);
      } else {
        getNext(newCoord, Direction::SOUTH, path);
      }
    } else if (boardPiece == '-') {
      if (dir == Direction::EAST) {
        getNext(newCoord, Direction::EAST, path);
      } else {
        getNext(newCoord, Direction::WEST, path);
      }
    } else if (boardPiece == 'L') {
      if (dir == Direction::SOUTH) {
        getNext(newCoord, Direction::EAST, path);
      } else {
        getNext(newCoord, Direction::NORTH, path);
      }
    } else if (boardPiece == 'J') {
      if (dir == Direction::SOUTH) {
        getNext(newCoord, Direction::WEST, path);
      } else {
        getNext(newCoord, Direction::NORTH, path);
      }
    } else if (boardPiece == '7') {
      if (dir == Direction::NORTH) {
        getNext(newCoord, Direction::WEST, path);
      } else {
        getNext(newCoord, Direction::SOUTH, path);
      }
    } else if (boardPiece == 'F') {
      if (dir == Direction::NORTH) {
        getNext(newCoord, Direction::EAST, path);
      } else {
        getNext(newCoord, Direction::SOUTH, path);
      }
    } else if (boardPiece == '.') {
      paths.erase(path);
      return;
    } else if (boardPiece == 'S') {
      return;
    }
    return;
  }

  boardCoord getNewCoord(boardCoord coord, Direction dir) {
    switch (dir) {
    case Direction::NORTH:
      return {coord.first - 1, coord.second};
    case Direction::SOUTH:
      return {coord.first + 1, coord.second};
    case Direction::EAST:
      return {coord.first, coord.second + 1};
    case Direction::WEST:
      return {coord.first, coord.second - 1};
    }
  };

  void solve() {
    getNext(start, Direction::NORTH, 0);
    getNext(start, Direction::EAST, 1);
    getNext(start, Direction::SOUTH, 2);
    getNext(start, Direction::WEST, 3);
    int farthest = 0;
    int median = 0;
    for (int i = 0; i < 4; i++) {
      if (paths[i].size() % 2 == 0) {
        median = paths[i].size() / 2;
      } else {
        median = paths[i].size() / 2 + 1;
      }
      if (median > farthest) {
        path_idx = i;
        farthest = median;
      }
    }
    std::cout << "Test Day 10 - Part 1: " << farthest << " " << path_idx
              << std::endl;
  };

  void solve2() {
    std::set<boardCoord> path_perimeter = {};
    for (auto coord : paths[path_idx]) {
      path_perimeter.insert(coord);
    }
      for (auto row : board) {
      for (auto c : row) {
        std::cout << c;
      }
      std::cout << std::endl;
    }
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
        if (c == 'S') {
          start = {board.size(), row.size() - 1};
        }
      }
      board.push_back(row);
    }
    fin.close();
  }
};
