#include "aoc.h"

namespace day09
{
  REGISTER_DAY(day09, std::vector<std::vector<int>>, int);

  REGISTER_TEST_EXAMPLE(day09, ExampleInput, 1, 114);
  REGISTER_TEST        (day09, Input,        1, 1637452029);
  REGISTER_TEST_EXAMPLE(day09, ExampleInput, 2, 2);
  REGISTER_TEST        (day09, Input,        2, 908);

  READ_INPUT(input)
  {
    std::string str;
    std::vector<std::vector<int>> ints;
    while (std::getline(input, str))
    {
      std::vector<int> numbers;
      std::stringstream ss{str};
      int i;
      while(ss >> i)
        numbers.emplace_back(i);

      ints.emplace_back(numbers);
    }
    return ints;
  }

  bool AllZeros(const std::vector<int>& numbers)
  {
    for (auto& number : numbers)
    {
      if (number != 0)
        return false;
    }
    return true;
  }

  std::vector<int> GetDifference(const std::vector<int>& numbers)
  {
    std::vector<int> newNumbers{};
    for (int i = 0; i < numbers.size() - 1; i++)
    {
      newNumbers.emplace_back(numbers[i + 1] - numbers[i]);
    }
    return newNumbers;
  }

  int GetExtrapolateForward(const std::vector<int>& numbers)
  {
    if (AllZeros(numbers))
      return 0;

    return numbers.back() + GetExtrapolateForward(GetDifference(numbers));
  }

  int GetExtrapolateBackward(const std::vector<int>& numbers)
  {
    if (AllZeros(numbers))
      return 0;

    return numbers.front() - GetExtrapolateBackward(GetDifference(numbers));
  }

  OUTPUT1(input)
  {
    int sum = 0;
    for (auto& numbers : input)
    {
      sum += GetExtrapolateForward(numbers);
    }
    return sum;
  }

  OUTPUT2(input)
  {
    int sum = 0;
    for (auto& numbers : input)
    {
      sum += GetExtrapolateBackward(numbers);
    }
    return sum;
  }
}
