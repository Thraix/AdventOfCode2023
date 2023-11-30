#include "aoc.h"

namespace day14
{
  REGISTER_DAY(day14, std::vector<int>, int);

  REGISTER_TEST_EXAMPLE(day14, ExampleInput, 1, 0);
  REGISTER_TEST        (day14, Input,        1, 0);
  REGISTER_TEST_EXAMPLE(day14, ExampleInput, 2, 0);
  REGISTER_TEST        (day14, Input,        2, 0);

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
