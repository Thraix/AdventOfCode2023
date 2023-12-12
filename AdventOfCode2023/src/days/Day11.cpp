#include "aoc.h"

namespace day11
{
  REGISTER_DAY(day11, Array2D<char>, int64_t);

  REGISTER_TEST_EXAMPLE(day11, ExampleInput, 1, 374);
  REGISTER_TEST        (day11, Input,        1, 9947476);
  REGISTER_TEST_EXAMPLE(day11, ExampleInput, 2, 82000210);
  REGISTER_TEST        (day11, Input,        2, 519939907614);

  READ_INPUT(input)
  {
    return Input::ReadArray2D(input);
  }

  bool IsInbetween(int i1, int i2, int between)
  {
    int min = std::min(i1, i2);
    int max = std::max(i1, i2);
    return min < between && between < max;
  }

  int64_t GetSumPairs(const Array2D<char>& input, int expansion)
  {
    std::set<int> vertEmpty;
    std::set<int> horiEmpty;
    for (int y = 0; y < input.height; y++)
    {
      bool empty = true;
      for (int x = 0; x < input.width; x++)
      {
        if (input.Get(x, y) == '#')
        {
          empty = false;
          break;
        }
      }
      if (empty)
        horiEmpty.emplace(y);
    }
    for (int x = 0; x < input.width; x++)
    {
      bool empty = true;
      for (int y = 0; y < input.height; y++)
      {
        if (input.Get(x, y) == '#')
        {
          empty = false;
          break;
        }
      }
      if (empty)
        vertEmpty.emplace(x);
    }
    std::vector<Index2D> galaxies;
    input.Each([&](const Array2D<char>& map, Index2D index) {
      if (map.Get(index) == '#')
        galaxies.emplace_back(index);
    });

    int64_t sum = 0;
    for(int i = 0; i < galaxies.size(); i++)
    {
      for(int j = i + 1; j < galaxies.size(); j++)
      {
        sum += std::abs(galaxies[i].x - galaxies[j].x) + std::abs(galaxies[i].y - galaxies[j].y);
        for (auto& empty : vertEmpty)
        {
          if (IsInbetween(galaxies[i].x, galaxies[j].x, empty))
            sum += expansion;
        }
        for (auto& empty : horiEmpty)
        {
          if (IsInbetween(galaxies[i].y, galaxies[j].y, empty))
            sum += expansion;
        }
      }
    }

    return sum;
  }

  OUTPUT1(input)
  {
    return GetSumPairs(input, 1);
  }

  OUTPUT2(input)
  {
    return GetSumPairs(input, 999999);
  }
}
