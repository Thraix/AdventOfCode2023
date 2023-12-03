#include "aoc.h"

namespace day03
{
  REGISTER_DAY(day03, Array2D<char>, int);

  REGISTER_TEST_EXAMPLE(day03, ExampleInput, 1, 4361);
  REGISTER_TEST        (day03, Input,        1, 549908);
  REGISTER_TEST_EXAMPLE(day03, ExampleInput, 2, 467835);
  REGISTER_TEST        (day03, Input,        2, 81166799);

  bool HasAdjecent(const Array2D<char>& array, Index2D index, int width)
  {
    for (int y = index.y - 1; y <= index.y + 1; y++)
    {
      if (!array.IsInside(Index2D{index.x, y}))
        continue;

      for (int x = index.x - 1; x < index.x + width + 1; x++)
      {
        if (!array.IsInside(Index2D{x, y}))
          continue;

        if (array.Get(x, y) != '.' && !Helper::IsDigit(array.Get(x, y)))
          return true;
      }
    }
    return false;
  }

  int GetNumber(const Array2D<char>& array, Index2D index)
  {
    int number = 0;
    while (index.x < array.width && Helper::IsDigit(array.Get(index)))
    {
      number = number * 10 + array.Get(index) - '0';
      index.x++;
    }
    return number;
  }

  int GetGearRatio(const Array2D<char>& array, Index2D index)
  {
    int firstNum = 0;
    for (int y = index.y - 1; y <= index.y + 1; y++)
    {
      if (!array.IsInside(Index2D{index.x, y}))
        continue;

      for (int x = index.x - 1; x <= index.x + 1; x++)
      {
        if (!array.IsInside(Index2D{x, y}))
          continue;

        if (Helper::IsDigit(array.Get(x, y)))
        {
          Index2D numberStart = Index2D{x, y};
          while (numberStart.x >= 0 && Helper::IsDigit(array.Get(numberStart)))
            numberStart.x--;
          numberStart.x++;

          int number = GetNumber(array, numberStart);
          if (firstNum == 0)
            firstNum = number;
          else
            return firstNum * number;

          while (x < array.width && Helper::IsDigit(array.Get(x, y)))
            x++;
          x--;
        }
      }
    }


    return 0;
  }

  READ_INPUT(input)
  {
    return Input::ReadArray2D(input);
  }

  OUTPUT1(input)
  {
    int sum = 0;
    for (int y = 0; y < input.height; y++)
    {
      for (int x = 0; x < input.width; x++)
      {
        if(Helper::IsDigit(input.Get(x, y)))
        {
          int i = 1;
          while (x + i < input.width && Helper::IsDigit(input.Get(x + i, y))) i++;
          if (HasAdjecent(input, Index2D{x, y}, i))
          {
            sum += GetNumber(input, Index2D{x, y});
          }
          x += i - 1;
        }
      }
    }
    return sum;
  }

  OUTPUT2(input)
  {
    int sum = 0;
    input.Each([&](const Array2D<char>& array, const Index2D& index) {
      if (array.Get(index) == '*')
      {
        sum += GetGearRatio(input, index);
      }
    });
    return sum;
  }
}
