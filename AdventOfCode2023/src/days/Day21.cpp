#include "aoc.h"

namespace day21
{
  REGISTER_DAY(day21, Array2D<char>, int);

  REGISTER_TEST_EXAMPLE(day21, ExampleInput, 1, 47);
  REGISTER_TEST        (day21, Input,        1, 3651);
  REGISTER_TEST_EXAMPLE(day21, ExampleInput, 2, 2082783057);
  REGISTER_TEST        (day21, Input,        2, 607334325965751);

  READ_INPUT(input)
  {
    return Input::ReadArray2D(input);
  }

  int Visited(const Array2D<char>& map, Index2D start, int maxSteps)
  {
    if (maxSteps < 0)
      return 0;
    std::set<Index2D> exactly;
    std::set<Index2D> visited;
    std::queue<std::pair<Index2D, int>> stack{};
    stack.push(std::make_pair(start, 0));

    while (!stack.empty())
    {
      std::pair<Index2D, int> pos = stack.front();
      stack.pop();
      if (pos.second % 2 == maxSteps % 2)
      {
        exactly.emplace(pos.first);
      }
      if(pos.second == maxSteps)
        continue;

      for (auto& i : map.GetNeighbors(pos.first, false))
      {
        if (map.Get(i) != '#' && visited.emplace(i).second)
        {
          stack.push(std::make_pair(i, pos.second + 1));
        }
      }
    }
    return exactly.size();
  }

  OUTPUT1(input)
  {
    Index2D pos = input.Find('S');
    return Visited(input, {pos}, 64);
  }

  OUTPUT2(input)
  {
    int64_t steps = 26501365;
    int64_t halfStep = input.width / 2 + 1;
    int64_t boxes = steps / input.width - 1; // How many grids in any direction we will have with full traversal

    int64_t traversed = boxes * input.width + input.width / 2;

    int64_t totalOdd = (boxes / 2 + boxes / 2 + 1) * (boxes / 2 + boxes / 2 + 1);
    int64_t totalEven = 4 * ((boxes + 1) / 2) * ((boxes + 1) / 2);

    int64_t stepsLeft = steps - traversed - 1;

    // Calculate the amount of positions we can get to if we traverse the entire grid
    int64_t odds = Visited(input, {Index2D{input.width / 2, input.height / 2}}, 2 * input.width + steps);
    int64_t evens = Visited(input, {Index2D{input.width / 2, input.height / 2 + 1}}, 2 * input.width + steps);
    int64_t fullGridsTotal = odds * (int64_t)totalOdd + evens * (int64_t)totalEven;

    // Corner case handling
    int64_t TL = Visited(input, Index2D{input.width - 1, input.height - 1}, stepsLeft + halfStep - 1);
    int64_t TR = Visited(input, Index2D{0, input.height - 1}, stepsLeft + halfStep - 1);
    int64_t BL = Visited(input, Index2D{input.width - 1, 0}, stepsLeft + halfStep - 1);
    int64_t BR = Visited(input, Index2D{0, 0}, stepsLeft + halfStep - 1);

    int64_t TL2 = Visited(input, Index2D{input.width - 1, input.height - 1}, stepsLeft - halfStep);
    int64_t TR2 = Visited(input, Index2D{0, input.height - 1}, stepsLeft - halfStep);
    int64_t BL2 = Visited(input, Index2D{input.width - 1, 0}, stepsLeft - halfStep);
    int64_t BR2 = Visited(input, Index2D{0, 0}, stepsLeft - halfStep);
    int64_t cornerTotal = (TL + TR + BL + BR) * boxes + (TL2 + TR2 + BL2 + BR2) * (int64_t)(boxes + 1);

    // Endpoint handling
    int64_t L = Visited(input, Index2D{input.width - 1, input.height / 2}, stepsLeft);
    int64_t T = Visited(input, Index2D{input.width / 2, input.height - 1}, stepsLeft);
    int64_t R = Visited(input, Index2D{0, input.height / 2}, stepsLeft);
    int64_t B = Visited(input, Index2D{input.width / 2, 0}, stepsLeft);

    int64_t L2 = Visited(input, Index2D{input.width - 1, input.height / 2}, stepsLeft - input.width);
    int64_t T2 = Visited(input, Index2D{input.width / 2, input.height - 1}, stepsLeft - input.width);
    int64_t R2 = Visited(input, Index2D{0, input.height / 2}, stepsLeft - input.width);
    int64_t B2 = Visited(input, Index2D{input.width / 2, 0}, stepsLeft - input.width);
    int64_t edgeTotal = L + T + R + B + L2 + T2 + R2 + B2;

    return fullGridsTotal + cornerTotal + edgeTotal;
  }
}
