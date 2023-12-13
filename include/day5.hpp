#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

// seed-to-soil map:
// 50 98 2
// ^   ^  ^
// dr sr rl
//
typedef std::vector<std::tuple<long, long, long>> InstructionVec;

enum InputState {
  SEED,
  SOIL,
  FERTILIZER,
  WATER,
  LIGHT,
  TEMPERATURE,
  HUMIDITY,
  LOCATION,
  DEFAULT
};

class Day5 {
  std::vector<long> range_seeds;
  std::vector<long> seeds_loc;
  std::vector<long> seeds;
  InstructionVec soil;
  InstructionVec fertilizer;
  InstructionVec water;
  InstructionVec light;
  InstructionVec temperature;
  InstructionVec humidity;
  InstructionVec location;
  InputState input_state = InputState::DEFAULT;

public:
  Day5() { loadInput("inputs/day5.txt"); };
  ~Day5(){};

  void addTupleToVec(long dest, long src, long len) {
    std::tuple<long, long, long> value = {dest, src, len};
    switch (input_state) {
    case InputState::SOIL:
      soil.push_back(value);
      break;
    case InputState::FERTILIZER:
      fertilizer.push_back(value);
      break;
    case InputState::WATER:
      water.push_back(value);
      break;
    case InputState::LIGHT:
      light.push_back(value);
      break;
    case InputState::TEMPERATURE:
      temperature.push_back(value);
      break;
    case InputState::HUMIDITY:
      humidity.push_back(value);
      break;
    case InputState::LOCATION:
      location.push_back(value);
      break;
    case InputState::SEED:
      break;
    case InputState::DEFAULT:
      break;
    };
  };

  void loadInput(const std::string &filename) {
    std::ifstream fin(filename);
    std::string line;
    while (std::getline(fin, line)) {
      long digit = -1;
      long digit_dest = -1;
      long digit_src = -1;
      for (int i = 0; i <= line.size(); i++) {
        char ch = line[i];
        if (ch == 's' && line.substr(i, 5) == "seeds") {
          input_state = InputState::SEED;
          continue;
        }
        if (ch == 's' && line.substr(i, 12) == "seed-to-soil") {
          input_state = InputState::SOIL;
          continue;
        }
        if (ch == 's' && line.substr(i, 18) == "soil-to-fertilizer") {
          input_state = InputState::FERTILIZER;
          continue;
        }
        if (ch == 'f' && line.substr(i, 19) == "fertilizer-to-water") {
          input_state = InputState::WATER;
          continue;
        }
        if (ch == 'w' && line.substr(i, 14) == "water-to-light") {
          input_state = InputState::LIGHT;
          continue;
        }
        if (ch == 'l' && line.substr(i, 20) == "light-to-temperature") {
          input_state = InputState::TEMPERATURE;
          continue;
        }
        if (ch == 't' && line.substr(i, 23) == "temperature-to-humidity") {
          input_state = InputState::HUMIDITY;
          continue;
        }
        if (ch == 'h' && line.substr(i, 20) == "humidity-to-location") {
          input_state = InputState::LOCATION;
          continue;
        }
        if (ch == ' ' && digit > -1) {
          if (input_state == InputState::SEED) {
            seeds.push_back(digit);
          }
          if (digit_dest == -1) {
            digit_dest = digit;
          } else {
            digit_src = digit;
          }
          digit = -1;
        }

        if (std::isdigit(ch)) {
          std::string s_digit = std::to_string(digit);
          if (digit == -1) {
            s_digit = '0';
          }
          digit = std::stol(s_digit + ch);
        }
        if (i == line.size() && digit > -1) {
          if (input_state == InputState::SEED) {
            seeds.push_back(digit);
          } else {
            addTupleToVec(digit_dest, digit_src, digit);
          }
          digit = -1;
          digit_dest = -1;
          digit_src = -1;
        }
      }
    }
  }

  long applyVec(long seed, InstructionVec &vec) {
    for (auto instruction : vec) {
      long dest = std::get<0>(instruction);
      long src = std::get<1>(instruction);
      long len = std::get<2>(instruction);
      if (seed >= src && seed < src + len) {
        long offset = seed - src;
        return dest + offset;
      }
    }
    return seed;
  };
  long applyReverseVec(long seed, InstructionVec &vec) {
    for (auto instruction : vec) {
      long dest = std::get<1>(instruction);
      long src = std::get<0>(instruction);
      long len = std::get<2>(instruction);
      if (seed >= src && seed < src + len) {
        long offset = seed - src;
        return dest + offset;
      }
    }
    return seed;
  };

  void process1() {
    for (auto seed : seeds) {
      long soil_v = applyVec(seed, soil);
      long fertilizer_v = applyVec(soil_v, fertilizer);
      long water_v = applyVec(fertilizer_v, water);
      long light_v = applyVec(water_v, light);
      long temperature_v = applyVec(light_v, temperature);
      long humidity_v = applyVec(temperature_v, humidity);
      long location_v = applyVec(humidity_v, location);
      seeds_loc.push_back(location_v);
    }
    std::cout << "Test Day 5 - Part 1 " << std::endl;

    for (auto loc : seeds_loc) {
      std::cout << loc << " ";
    }
    std::cout << std::endl;
    std::cout << "Min location: "
              << *std::min_element(seeds_loc.begin(), seeds_loc.end())
              << std::endl;
  };

  void process2() {
    for (int i = 4; i < seeds.size(); i += 2) {
      std::cout << i << std::endl;
      for (int j = seeds[i]; j < seeds[i] + seeds[i + 1]; j++) {
        range_seeds.push_back(j);
      }
    }
    std::printf("end range\n");
    for (long seed : range_seeds) {
      long soil_v = applyVec(seed, soil);
      long fertilizer_v = applyVec(soil_v, fertilizer);
      long water_v = applyVec(fertilizer_v, water);
      long light_v = applyVec(water_v, light);
      long temperature_v = applyVec(light_v, temperature);
      long humidity_v = applyVec(temperature_v, humidity);
      long location_v = applyVec(humidity_v, location);
      seeds_loc.push_back(location_v);
    }
    std::cout << "Test Day 5 - Part 2 " << std::endl;
    std::cout << "Min location: "
              << *std::min_element(seeds_loc.begin(), seeds_loc.end())
              << std::endl;
  }

  void print() {
    std::printf("seeds: \n");
    for (auto seed : seeds) {
      std::cout << seed << " ";
    }
    std::cout << std::endl;
    std::printf("soil: \n");
    for (auto el : soil) {
      std::cout << std::get<0>(el) << " " << std::get<1>(el) << " "
                << std::get<2>(el) << " ";
      std::cout << std::endl;
    }
    std::printf("fertilizer: \n");
    for (auto el : fertilizer) {
      std::cout << std::get<0>(el) << " " << std::get<1>(el) << " "
                << std::get<2>(el) << " ";
      std::cout << std::endl;
    }
    std::printf("water: \n");
    for (auto el : water) {
      std::cout << std::get<0>(el) << " " << std::get<1>(el) << " "
                << std::get<2>(el) << " ";
      std::cout << std::endl;
    }
    std::printf("light: \n");
    for (auto el : light) {
      std::cout << std::get<0>(el) << " " << std::get<1>(el) << " "
                << std::get<2>(el) << " ";
      std::cout << std::endl;
    }
    std::printf("temperature: \n");
    for (auto el : temperature) {
      std::cout << std::get<0>(el) << " " << std::get<1>(el) << " "
                << std::get<2>(el) << " ";
      std::cout << std::endl;
    }
    std::printf("location: \n");
    for (auto el : location) {
      std::cout << std::get<0>(el) << " " << std::get<1>(el) << " "
                << std::get<2>(el) << " ";
      std::cout << std::endl;
    }
  };

  void test() {
    // print();
    process1();
  };
};
