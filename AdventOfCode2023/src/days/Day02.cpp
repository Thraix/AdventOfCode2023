#include "aoc.h"

namespace day02
{
  struct Set
  {
    int red = 0;
    int green = 0;
    int blue = 0;
  };

  REGISTER_DAY(day02, std::vector<std::vector<Set>>, int);

  REGISTER_TEST_EXAMPLE(day02, ExampleInput, 1, 8);
  REGISTER_TEST        (day02, Input,        1, 2164);
  REGISTER_TEST_EXAMPLE(day02, ExampleInput, 2, 2286);
  REGISTER_TEST        (day02, Input,        2, 69929);

  READ_INPUT(input)
  {
    std::string str;
    std::vector<std::vector<Set>> games;
    while (std::getline(input, str))
    {
      std::vector<Set> sets{Set{}};
      std::stringstream ss{str};
      int i = 0;
      ss >> "Game " >> i >> ": ";
      while (true)
      {
        int count;
        std::string col;
        ss >> count >> col;
        if (col.front() == 'r') sets.back().red = count;
        if (col.front() == 'g') sets.back().green = count;
        if (col.front() == 'b') sets.back().blue = count;
        if (col.back() == ';') sets.emplace_back(Set{});
        else if (col.back() == ',');
        else break;
      }
      games.emplace_back(sets);
    }
    return games;
  }

  OUTPUT1(input)
  {
    int sum = 0;
    for (int i = 0; i < input.size(); i++)
    {
      bool valid = true;
      for (auto& set : input[i])
      {
        if(set.red > 12 || set.green > 13 || set.blue > 14)
        {
          valid = false;
          break;
        }
      }
      if (valid)
        sum += i + 1;
    }
    return sum;
  }

  OUTPUT2(input)
  {
    int sum = 0;
    for (int i = 0; i < input.size(); i++)
    {
      Set minSet;
      for (auto& set : input[i])
      {
        minSet.red = std::max(minSet.red, set.red);
        minSet.green = std::max(minSet.green, set.green);
        minSet.blue = std::max(minSet.blue, set.blue);
      }
      sum += minSet.red * minSet.green * minSet.blue;
    }
    return sum;
  }
}
