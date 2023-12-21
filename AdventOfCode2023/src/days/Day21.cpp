#include "aoc.h"

namespace day21
{
  REGISTER_DAY(day21, Array2D<char>, int);

  REGISTER_TEST_EXAMPLE(day21, ExampleInput, 1, 16);
  // REGISTER_TEST        (day21, Input,        1, 3651);
  // REGISTER_TEST_EXAMPLE(day21, ExampleInput, 2, 16733044);
  REGISTER_TEST        (day21, Input,        2, 0);

  READ_INPUT(input)
  {
    return Input::ReadArray2D(input);
  }

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
      }
        if(pos.second == input.width * 2)
          continue;

      std::vector<Index2D> directions{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

      for (auto& i : directions)
      {
        if (visited.emplace(pos.first + i).second)
        // if (input.GetMod(pos.first.x + i.x, pos.first.y + i.y) != '#' && visited.emplace(pos.first + i).second)
        {
          stack.push(std::make_pair(pos.first + i, pos.second + 1));
        }
      }
    }

    Array2D<char> cpy{input.width * 5, input.height * 5, '.'};
    for(auto& i : exactly)
    {
      cpy.Set(i.x + input.width * 2, i.y + input.height * 2, 'O');
    }
    std::cout << cpy << std::endl;

    return exactly.size();
  }

  struct State
  {
    Index2D inPos;
    int costToFill;
    std::vector<std::pair<int, Index2D>> costToNeighbor;
  };

  struct Res
  {
    int costToFill;
    int maxVisited;
    std::vector<std::pair<Index2D, int>> neighborPathCost;
  };

  
  int64_t GetVisitedSquares(const Array2D<char>& map, Index2D startPos, int stepsLeft, std::map<Index2D, Res>& states)
  {
    // Index2D incoming pos, cost to fill, list of resulting neighbor pos and cost
    auto it = states.find(startPos);
    if (it != states.end())
    {
      if (it->second.costToFill < stepsLeft)
      {
        int64_t sum = 0;
        for (auto& next : it->second.neighborPathCost)
        {
          sum += GetVisitedSquares(map, next.first, stepsLeft - next.second, states);
        }
        return sum + it->second.maxVisited;
      }
    }

    std::set<Index2D> exactly;
    std::set<Index2D> visited;
    std::map<Index2D, std::pair<Index2D, int>> costToNeighbor; std::queue<std::pair<Index2D, int>> stack{}; stack.push(std::make_pair(startPos, 0));
    bool ranOutOfSteps = false;
    int maxSteps = 0;

    while (!stack.empty())
    {

      std::pair<Index2D, int> pos = stack.front();
      stack.pop();
      maxSteps = std::max(maxSteps, pos.second);
      if (pos.second % 2 == stepsLeft % 2)
      {
        exactly.emplace(pos.first);
        if (pos.second == stepsLeft)
        {
          ranOutOfSteps = true;
          continue;
        }
      }

      std::vector<Index2D> directions{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

      for (auto& i : directions)
      {
        if (map.IsInside(pos.first + i))
        {
          // if (map.Get(pos.first + i) != '#' && visited.emplace(pos.first + i).second)
          if (visited.emplace(pos.first + i).second)
          {
            stack.push(std::make_pair(pos.first + i, pos.second + 1));
          }
        }
        else
        {
          costToNeighbor.emplace(i, std::make_pair(pos.first + i, pos.second));
        }
      }
    }
    std::vector<std::pair<Index2D, int>> neighbor;
    for (auto& costToNeigh : costToNeighbor)
    {
      Index2D pos = costToNeigh.second.first;
      pos.x = ((pos.x % map.width) + map.width) % map.width;
      pos.y = ((pos.y % map.height) + map.height) % map.height;
      neighbor.emplace_back(std::make_pair(pos, costToNeigh.second.second));
    }

    if(!ranOutOfSteps)
      states.emplace(startPos, Res{maxSteps, (int)exactly.size(), neighbor});

    int sum = 0;
    for (auto& next : neighbor)
    {
      sum += GetVisitedSquares(map, next.first, stepsLeft - next.second, states);
    }

    return sum + exactly.size();
  }

  OUTPUT2(input)
  {
    int steps = 33;
    int halfStep = input.width / 2;
    int fullTraversal = input.width / 2 + input.width - 1;
    int boxes = (steps - halfStep) / fullTraversal; // How many grids in any direction we will have with full traversal







    int filledWidth = (steps / input.width - 1) / 2;
    Index2D pos = input.Find('S');
    std::map<Index2D, Res> states;
    // return GetVisitedSquares(input, pos, 5000, states);
    return 0;
  }
}


// ... .O. ...
// ... ... ...
// ..O .O. O..
// 
// ... O.O ...
// O.O .O. O.O
// ... O.O ...
// 
// ..O .O. O..
// ... ... ...
// ... .O. ...
