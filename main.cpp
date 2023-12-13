#include "include/day1.hpp"
#include "include/day2.hpp"
#include "include/day3.hpp"
#include "include/day4.hpp"
#include "include/day5.hpp"
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

  return 0;
}
