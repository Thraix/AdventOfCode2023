#include "aoc.h"

namespace day06
{
  using Game = std::pair<int, int>;
  REGISTER_DAY(day06, std::vector<Game>, int);

  REGISTER_TEST_EXAMPLE(day06, ExampleInput, 1, 288);
  REGISTER_TEST        (day06, Input,        1, 4811940);
  REGISTER_TEST_EXAMPLE(day06, ExampleInput, 2, 71503);
  REGISTER_TEST        (day06, Input,        2, 30077773);

  READ_INPUT(input)
  {
    std::string str;
    std::vector<std::pair<int, int>> games;
    std::getline(input, str);
    std::stringstream ss{str};
    int i;
    ss >> "Time: ";
    while (ss >> i) games.emplace_back(i, 0);

    std::getline(input, str);
    std::stringstream ss2{str};
    ss2 >> "Distance: ";

    int count = 0;
    while (ss2 >> i)
    {
      games[count].second = i;
      count++;
    }
    return games;
  }

  OUTPUT1(input)
  {
    int product = 1;
    for(auto& [time, distance] : input)
    {
      int lowerBound = std::floor(time / 2.0 - sqrt(time * time / 4.0 - distance)) + 1;
      int upperBound = std::ceil(time / 2.0 + sqrt(time * time / 4.0 - distance)) - 1;

      product *= upperBound - lowerBound + 1;
    }
    return product;
  }

  OUTPUT2(input)
  {
    int64_t time = 0;
    int64_t distance = 0;
    std::string sTime = "";
    std::string sDistance = "";
    for (auto& game : input)
    {
      sTime += std::to_string(game.first);
      sDistance += std::to_string(game.second);
    }
    char* endPtr;
    time = std::strtoll(sTime.c_str(), &endPtr, 10);
    distance = std::strtoll(sDistance.c_str(), &endPtr, 10);

    int lowerBound = std::floor(time / 2.0 - sqrt(time * time / 4.0 - distance)) + 1;
    int upperBound = std::ceil(time / 2.0 + sqrt(time * time / 4.0 - distance)) - 1;

    return upperBound - lowerBound + 1;
  }
}
