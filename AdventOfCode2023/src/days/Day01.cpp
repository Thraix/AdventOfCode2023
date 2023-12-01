#include "aoc.h"

namespace day01
{
  REGISTER_DAY(day01, std::vector<std::string>, int);

  REGISTER_TEST_EXAMPLE(day01, ExampleInput, 1, 209);
  REGISTER_TEST        (day01, Input,        1, 54990);
  REGISTER_TEST_EXAMPLE(day01, ExampleInput, 2, 281);
  REGISTER_TEST        (day01, Input,        2, 54473);


  int GetNumberAt(const std::string& str, int pos)
  {
    if(str[pos] >= '0' && str[pos] <= '9')
    {
      return str[pos] - '0';
    }
    else
    {
      static const std::vector<std::string> numbers{"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
      for (int num = 0; num < numbers.size(); num++)
      {
        if (Helper::StartsWith(std::string_view{&str[pos], str.size() - pos}, numbers[num]))
        {
          return num + 1;
        }
      }
    }
    return -1;
  }

  READ_INPUT(input)
  {
    return Input::ReadLines(input);
  }

  OUTPUT1(input)
  {
    int sum = 0;
    for (auto& str : input)
    {
      int first = 0;
      int last = 0;
      for (int i = 0; i < str.size(); i++)
      {
        if(Helper::IsDigit(str[i]))
        {
          first = str[i] - '0';
          break;
        }
      }
      for (int i = str.size() - 1; i >= 0; i--)
      {
        if(Helper::IsDigit(str[i]))
        {
          last = str[i] - '0';
          break;
        }
      }
      sum += first * 10 + last;
    }
    return sum;
  }

  OUTPUT2(input)
  {
    int sum = 0;
    for (auto& str : input)
    {
      int first = -1;
      int last = -1;
      for (int i = 0; i < str.size(); i++)
      {
        int number = GetNumberAt(str, i);
        if (number != -1)
        {
          first = number;
          break;
        }
      }
      for (int i = str.size() - 1; i >= 0; i--)
      {
        int number = GetNumberAt(str, i);
        if (number != -1)
        {
          last = number;
          break;
        }
      }
      sum += first * 10 + last;
    }
    return sum;
  }
}
