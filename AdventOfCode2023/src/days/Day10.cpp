#include "aoc.h"

namespace day10
{
  REGISTER_DAY(day10, std::vector<int>, int);

  REGISTER_TEST_EXAMPLE(day10, ExampleInput, 1, 0);
  REGISTER_TEST        (day10, Input,        1, 0);
  REGISTER_TEST_EXAMPLE(day10, ExampleInput, 2, 0);
  REGISTER_TEST        (day10, Input,        2, 0);

  READ_INPUT(input)
  {
    std::string str;
    std::vector<int> ints;
    while (std::getline(input, str))
    {
        ints.emplace_back(std::stoi(str));
    }
    return ints;
  }

  OUTPUT1(input)
  {
    return 0;
  }

  OUTPUT2(input)
  {
    return 0;
  }
}
