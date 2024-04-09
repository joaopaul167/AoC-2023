#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
class Day9 {
public:
  std::vector<std::vector<long>> m;
  Day9() {
    parseInput("inputs/day9.txt");
    solve();
  };
  int extrapolate(const std::vector<long> &v, bool forward = true) {
    std::vector<long> diff(v.size() - 1);
    for (int i = 0; i < v.size() - 1; ++i) {
      diff[i] = v[i + 1] - v[i];
    }
    bool allZeros = std::all_of(diff.begin(), diff.end(),
                                [](int element) { return element == 0; });
    int acc = allZeros ? 0 :  extrapolate(diff, forward);
    return forward ? v.back() + acc : v.front() - acc;
  };

  void solve() {
    int sum = 0;
    int sum2 = 0;
    for (auto &p : m) {
      sum += extrapolate(p);
      sum2 += extrapolate(p, false);
    }
    std::cout << "test day 9 - part 1: " << sum << std::endl;
    std::cout << "test day 9 - part 2: " << sum2 << std::endl;
  };

  void parseInput(const std::string &filename) {
    std::ifstream fin(filename);
    std::string line;
    while (std::getline(fin, line)) {
      std::istringstream iss(line);
      int num;
      std::vector<long> v;
      while (iss >> num) {
        v.push_back(num);
      }
      m.push_back(v);
    }
    fin.close();
  }
};
