#include "include/day1.hpp"
#include "include/day2.hpp"
#include "include/day3.hpp"
#include "include/day4.hpp"
#include "include/day5.hpp"
#include "include/day6.hpp"
#include "include/day7.hpp"
#include "include/day8.hpp"
#include "include/day9.hpp"
#include "include/day10.hpp"
#include "include/day11.hpp"
#include "include/day12.hpp"
#include <map>
#include <memory>

int main() {
  Day1 t1;
  t1.test();

  Day2 t2;
  auto g = std::make_shared<Game>(12,13,14);
  t2.test(g);

  Day3 t3;
  t3.test();

  Day4 t4;
  t4.test();

  Day5 t5;
  t5.test();

  Day6 t6;

  Day7 t7;
  
  Day8 t8;

  Day9 t9;

  Day10 t10;

  Day11 t11;

  Day12<void(std::string, std::map<std::string,long>) ,std::string> t12;

  return 0;
}
