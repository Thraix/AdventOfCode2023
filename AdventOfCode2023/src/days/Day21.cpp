#include "aoc.h"

namespace day21
{
  REGISTER_DAY(day21, Array2D<char>, int);

  // REGISTER_TEST_EXAMPLE(day21, ExampleInput, 1, 16);
  REGISTER_TEST        (day21, Input,        1, 3651);
  REGISTER_TEST_EXAMPLE(day21, ExampleInput, 2, 0);
  REGISTER_TEST        (day21, Input,        2, 0);

  READ_INPUT(input)
  {
    return Input::ReadArray2D(input);
  }

  // 1282 too low
  OUTPUT1(input)
  {
    Index2D pos = input.Find('S');

    std::set<Index2D> exactly;
    std::set<Index2D> visited;
    std::queue<std::pair<Index2D, int>> stack{};
    stack.push(std::make_pair(pos, 0));

    while (!stack.empty())
    {
      std::pair<Index2D, int> pos = stack.front();
      stack.pop();
      if (pos.second % 2 == 0)
      {
        exactly.emplace(pos.first);
        if(pos.second == 64)
          continue;
      }

      std::vector<Index2D> directions{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

      for (auto& i : directions)
      {
        if (input.GetMod(pos.first.x + i.x, pos.first.y + i.y) != '#' && visited.emplace(pos.first + i).second)
        {
          stack.push(std::make_pair(pos.first + i, pos.second + 1));
        }
      }
    }

    auto cpy = input;
    for(auto& i : exactly)
    {
      cpy.SetMod(i.x, i.y, 'O');
    }
    std::cout << cpy << std::endl;

    return exactly.size();
  }

  OUTPUT2(input)
  {
#if 0
    Index2D pos = input.Find('S');

    std::set<Index2D> exactly;
    std::set<Index2D> visited;
    std::queue<std::pair<Index2D, int>> stack{};
    stack.push(std::make_pair(pos, 0));

    while (!stack.empty())
    {
      std::pair<Index2D, int> pos = stack.front();
      stack.pop();
      if (pos.second % 2 == 0)
      {
        exactly.emplace(pos.first);
        if(pos.second == 5000) //26501365)
          continue;
      }

      std::vector<Index2D> directions{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

      for (auto& i : directions)
      {
        Index2D newI{pos.first + i}:
        if (input.GetMod(pos.first.x + i.x, pos.first.x + i.y) != '#' && visited.emplace(pos.first + i).second)
        {
          stack.push(std::make_pair(pos + i, pos.second + 1));
        }
      }
    }

    auto cpy = input;
    for(auto& i : exactly)
    {
      cpy.Set(i.x, i.y, 'O');
    }
    std::cout << cpy << std::endl;

    return exactly.size();
#endif
    return 0;
  }
}
