#include "aoc.h"

namespace day22
{
  REGISTER_DAY(day22, std::vector<int>, int);

  REGISTER_TEST_EXAMPLE(day22, ExampleInput, 1, 0);
  REGISTER_TEST        (day22, Input,        1, 0);
  REGISTER_TEST_EXAMPLE(day22, ExampleInput, 2, 0);
  REGISTER_TEST        (day22, Input,        2, 0);

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
