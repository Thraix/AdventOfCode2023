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

      for (auto& i : input.GetNeighbors(pos.first, false))
      {
        if (input.Get(i) != '#' && visited.emplace(i).second)
        {
          stack.push(std::make_pair(i, pos.second + 1));
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
  }

  OUTPUT2(input)
  {
    int steps = 26501365;

    int gridsFilled = steps / 0;
    return 0;
  }
}
