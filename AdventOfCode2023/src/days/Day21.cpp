#include "aoc.h"

namespace day21
{
  REGISTER_DAY(day21, std::vector<int>, int);

  REGISTER_TEST_EXAMPLE(day21, ExampleInput, 1, 0);
  REGISTER_TEST        (day21, Input,        1, 0);
  REGISTER_TEST_EXAMPLE(day21, ExampleInput, 2, 0);
  REGISTER_TEST        (day21, Input,        2, 0);

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
