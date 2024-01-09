// The organizer explains that it's not really a traditional race - instead, 
// you will get a fixed amount of time during which your boat has to travel 
// as far as it can, and you win if your boat goes the farthest.
//
#include <iostream>
#include <string>
#include <vector>
class Day6 {
  // time / target distance
  std::vector<long> time{ 44707080 };
  std::vector<long> t_distance{ 283113411341491 };
  std::vector<long> distance{};
public:
  Day6(){
    parseInput("inputs/day6.txt");
    solvePart1();
  };
  void solvePart1(){
    for (int i = 0; i < time.size(); i++) {
      long cur_t = time[i];      
      long cur_td = t_distance[i];
      long count = 0;      
      for (int j = 1; j <= cur_t; j++) {
        long cur_d = j * (cur_t - j);
        if (cur_d > cur_td) {
          count++;  
        }
      }
      distance.push_back(count);
    }
    // multiply evvery element in vector
    int result = 1;
    for (int i = 0; i < distance.size(); i++) {
      result *= distance[i];
    }
    std::cout << "Test Day 6 - Part 2: " << result << std::endl;

  };
  void solvePart2(){};
  void parseInput(const std::string &filename) {}
  void print(){
    for (int i = 0; i < time.size(); i++) {
      std::cout << time[i] << " " << t_distance[i] << std::endl;
    }  
  };
};
