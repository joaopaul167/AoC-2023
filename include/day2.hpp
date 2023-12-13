// You play several games and record the information from each game
// (your puzzle input). Each game is listed with its ID number
// (like the 11 in Game 11: ...) followed by a semicolon-separated
// list of subsets of cubes that were revealed from the bag
// (like 3 red, 5 green, 4 blue).
// For example, the record of a few games might look like this:
//
// Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
// Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue
// Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red
// Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red
// Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green
//
// In game 1, three sets of cubes are revealed from the bag (and then
// put back again). The first set is 3 blue cubes and 4 red cubes;
// the second set is 1 red cube, 2 green cubes, and 6 blue cubes;
// the third set is only 2 green cubes.
// The Elf would first like to know which games would have been possible
// if the bag contained only 12 red cubes, 13 green cubes, and 14 blue cubes?
// In the example above, games 1, 2, and 5 would have been possible if the bag
// had been loaded with that configuration. However, game 3 would have been
// impossible because at one point the Elf showed you 20 red cubes at once;
// similarly, game 4 would also have been impossible because the Elf showed
// you 15 blue cubes at once. If you add up the IDs of the games that would
// have been possible, you get 8.
//
// Determine which games would have been possible if the bag had been
// loaded with only 12 red cubes, 13 green cubes, and 14 blue cubes.
//
// What is the sum of the IDs of those games?
#pragma once
#include <cctype>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

class Game {
public:
  int r, g, b;
  Game() : r{0}, g{0}, b{0} {};
  Game(int ir, int ig, int ib) : r{ir}, g{ig}, b{ib} {};
  ~Game(){};
};

enum COLOR { RED, GREEN, BLUE };

class Day2 {
  clock_t begin_t;
  int acc = 0;
  std::map<int, std::shared_ptr<Game>> map;

public:
  Day2() {
    begin_t = std::clock();
    loadInput("inputs/day2.txt");
  };
  ~Day2(){};

  void addToGame(std::shared_ptr<Game> g, std::vector<std::string> &v,
                 COLOR type) {
    if (!std::isdigit(v[v.size() - 2][0]))
      return;
    int digit = std::stoi(v[v.size() - 2]);
    if (type == COLOR::BLUE) {
      if (g->b < digit) {
        g->b = digit;
      }
    } else if (type == COLOR::RED) {
      if (g->r < digit) {
        g->r = digit;
      }
    } else if (type == COLOR::GREEN) {
      if (g->g < digit) {
        g->g = digit;
      }
    }
  }

  void loadInput(const std::string &filename) {
    std::ifstream fin(filename);
    std::string line;
    while (std::getline(fin, line)) {
      acc++;
      auto g = std::make_shared<Game>();
      std::stringstream ss(line);
      std::string component;
      std::vector<std::string> vh;
      while (ss >> component) {
        acc++;
        vh.push_back(component);
        if (component.substr(0, 4) == "blue") {
          addToGame(g, vh, COLOR::BLUE);
        }
        if (component.substr(0, 3) == "red") {
          addToGame(g, vh, COLOR::RED);
        }
        if (component.substr(0, 5) == "green") {
          addToGame(g, vh, COLOR::GREEN);
        }
      }
      vh[1].erase(std::remove(vh[1].begin(), vh[1].end(), ':'), vh[1].end());
      map[std::stoi(vh[1])] = g;
    }
  }

  void test(std::shared_ptr<Game> g_ref) {
    int sum = 0;
    int sum_power = 0;
    for (auto i : map) {
      acc++;
      auto game = i.second;
      if (game->r <= g_ref->r && game->g <= g_ref->g && game->b <= g_ref->b) {
        sum += i.first;
      }
      sum_power += game->r * game->g * game->b;
    }
    std::cout << "Test Day 2: Part 1 sum: " << sum << std::endl;
    std::cout << "Test Day 2: Part 2 sum: " << sum_power << std::endl;
    std::cout << "Acc = " << acc << std::endl;
    std::cout << "Compute time: "
              << float(begin_t - std::clock()) / CLOCKS_PER_SEC << std::endl;
  }
};
