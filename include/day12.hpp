#pragma once
#include "task_scheduler.hpp"
#include <atomic>
#include <cstdio>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

// For each row, the condition records show every spring and whether it is
// operational (.) or damaged (#). This is the part of the condition records
// that is itself damaged; for some springs, it is simply unknown (?) whether
// the spring is operational or damaged.

// However, the engineer that produced the condition records also duplicated
// some of this information in a different format! After the list of springs for
// a given row, the size of each contiguous group of damaged springs is listed
// in the order those groups appear in the row. This list always accounts for
// every damaged spring, and each number is the entire size of its contiguous
// group (that is, groups are always separated by at least one operational
// spring: #### would always be 4, never 2,2).

// So, condition records with no unknown spring conditions might look like this:

// #.#.### 1,1,3
// .#...#....###. 1,1,3
// .#.###.#.###### 1,3,1,6
// ####.#...#... 4,1,1
// #....######..#####. 1,6,5
// .###.##....# 3,2,1

// However, the condition records are partially damaged; some of the springs'
// conditions are actually unknown (?). For example:

// ???.### 1,1,3
// .??..??...?##. 1,1,3
// ?#?#?#?#?#?#?#? 1,3,1,6
// ????.#...#... 4,1,1
// ????.######..#####. 1,6,5
// ?###???????? 3,2,1

// Equipped with this information, it is your job to figure out how many
// different arrangements of operational and broken springs fit the given
// criteria in each row.

// For each row, count all of the different arrangements of operational and
// broken springs that meet the given criteria. What is the sum of those counts?

// PART 2
// As you look out at the field of springs, you feel like there are way more
// springs than the condition records list. When you examine the records, you
// discover that they were actually folded up this whole time!

// To unfold the records, on each row, replace the list of spring conditions
// with five copies of itself (separated by ?) and replace the list of
// contiguous groups of damaged springs with five copies of itself (separated by
// ,).
//
template <typename F, typename T> class Day12 {
private:
  std::atomic<int> sum;
  std::atomic<int> sum2;
  TaskScheduler<F, T> scheduler{1};

public:
  Day12() { loadInput("inputs/day12.txt"); };

  ~Day12(){};
  std::string printGroups(std::vector<int> groups) {
    return std::accumulate(std::next(groups.begin()), groups.end(),
                           std::to_string(groups.front()),
                           [](const std::string &a, int b) {
                             return a + ", " + std::to_string(b);
                           });
  }

  void loadInput(const std::string &filename) {
    std::ifstream fin(filename);
    std::string line;

    Task<F, T> task;

    task.function = [this](std::string raw_string,
                           std::map<std::string, long> memoMap) {
      std::function<int(std::string, std::vector<int>)> count_arr =
          [&](std::string row, std::vector<int> groups) -> int {
        // std::cout << row << " grps: " << printGroups(groups) << std::endl;

        if (memoMap[row] ) {
          return memoMap[row];
        }
        if (row.empty()) {
          return groups.empty() ? 1 : 0;
        }

        char current = row[0];
        int count_n = 0;
        if (current == '.') {
          count_n = count_arr(row.substr(1), groups);
        } else if (current == '?') {
          count_n = count_arr("." + row.substr(1), groups) +
                    count_arr("#" + row.substr(1), groups);
        } else {
          if (groups.size() == 0) {
            count_n = 0;
          } else {
            int cur_group = groups.front();
            if (cur_group <= row.length() &&
                std::all_of(row.begin(), row.begin() + cur_group,
                            [](char c) { return c == '#' || c == '?'; })) {
              groups.erase(groups.begin());
              if (cur_group == row.length()) {
                count_n = groups.empty() ? 1 : 0;
              } else if (row[cur_group] == '.') {
                count_n = count_arr(row.substr(1 + cur_group), groups);
              } else if (row[cur_group] == '?') {
                count_n = count_arr("." + row.substr(1 + cur_group), groups);
              } else {
                count_n = 0;
              }
            } else {
              count_n = 0;
            }
          }
        }
        memoMap[row] = count_n;
        return count_n;
      };

      size_t spacePos = raw_string.find(' ');
      int n_diff_arr = 0;
      int n_dif_arr2 = 0;
      if (spacePos != std::string::npos) {
        std::vector<int> groupSizes;
        std::string firstPart = raw_string.substr(0, spacePos);
        std::string secondPart = raw_string.substr(spacePos + 1);
        std::stringstream ss(secondPart);
        int num;
        while (ss >> num) {
          groupSizes.push_back(num);
          if (ss.peek() != ',') {
            break;
          }
          ss.ignore();
        }

        std::vector<int> unfolded_gs;
        std::string unfolded_fp;
        for (int i = 0; i < 5; ++i) {
          unfolded_fp += (i == 0 ? "" : "?") + firstPart;
          unfolded_gs.insert(unfolded_gs.end(), groupSizes.begin(),
                             groupSizes.end());
        }
        n_diff_arr = count_arr(firstPart, groupSizes);
        n_dif_arr2 = count_arr(unfolded_fp, unfolded_gs);
      }
      sum.fetch_add(n_diff_arr, std::memory_order_relaxed);
      sum2.fetch_add(n_dif_arr2, std::memory_order_relaxed);
    };

    while (std::getline(fin, line)) {
      task.data = line;
      scheduler.addTask(task);
    }

    scheduler.waitForCompletion();
    std::printf("Test Day 12 - Part 1: %d \n", sum.load());
    std::printf("Test Day 12 - Part 2: %d \n", sum2.load());
  }
};
