#include "aoc.h"

namespace day23
{
  REGISTER_DAY(day23, Array2D<char>, int);

  REGISTER_TEST_EXAMPLE(day23, ExampleInput, 1, 94);
  REGISTER_TEST        (day23, Input,        1, 2130);
  REGISTER_TEST_EXAMPLE(day23, ExampleInput, 2, 154);
  REGISTER_TEST        (day23, Input,        2, 6710);

  READ_INPUT(input)
  {
    return Input::ReadArray2D(input);
  }

  struct State
  {
    Index2D pos;
    int cost;
    std::set<Index2D> visited;
  };

  struct State2
  {
    Index2D pos;
    int cost;

    Index2D lastIntersection;
    int lastIntersectionCost;
    std::set<Index2D> visited;
  };

  OUTPUT1(input)
  {
    int max = 0;
    std::stack<State> stack;
    stack.emplace(State{Index2D{1, 0}, 0, std::set<Index2D>{Index2D{1, 0}}});
    while(!stack.empty())
    {
      State state = stack.top();
      stack.pop();

      if (state.pos == Index2D{input.width - 2, input.height - 1}) {
        max = std::max(max, state.cost);
        continue;
      }

      for (auto& neighbor : input.GetNeighbors(state.pos, false))
      {
        if (state.visited.count(neighbor))
          continue;
        if (input.Get(neighbor) == '.' ||
           input.Get(neighbor) == '<' && neighbor.x < state.pos.x ||
           input.Get(neighbor) == '>' && neighbor.x > state.pos.x ||
           input.Get(neighbor) == 'v' && neighbor.y > state.pos.y ||
           input.Get(neighbor) == '^' && neighbor.y < state.pos.y)
        {
          std::set<Index2D> cpy = state.visited;
          cpy.emplace(state.pos);
          stack.emplace(State{neighbor, state.cost + 1, cpy});
        }
      }
    }
    return max;
  }

  // Takes 20+ minutes to solve. There is definitely a better solution somewhere
  OUTPUT2(input)
  {
    int max = 0;
    Graph<Index2D, int> graph;
    std::set<Index2D> visited;

    std::stack<State2> stack;
    stack.emplace(State2{Index2D{1, 0}, 0, Index2D{1, 0}, 0});
    while(!stack.empty())
    {
      State2 state = stack.top();
      stack.pop();

      if (state.pos == Index2D{input.width - 2, input.height - 1}) {
        graph.AddTwoWayEdge(state.lastIntersection, state.pos, state.cost - state.lastIntersectionCost);
        continue;
      }

      auto neighbors = input.GetNeighbors(state.pos, false);
      Index2D lastIntersection = state.lastIntersection;
      int lastIntersectionCost = state.lastIntersectionCost;
      int paths = 0;
      for(auto& neighbor : neighbors)
      {
        if(input.Get(neighbor) != '#')
          paths++;
      }
      if (paths > 2)
      {
        graph.AddTwoWayEdge(state.lastIntersection, state.pos, state.cost - state.lastIntersectionCost);
        lastIntersection = state.pos;
        lastIntersectionCost = state.cost;
        if (!visited.emplace(state.pos).second)
          continue;
      }
      for (auto& neighbor : input.GetNeighbors(state.pos, false))
      {
        std::set<Index2D> visitedState = state.visited;
        if (!visitedState.emplace(neighbor).second)
          continue;
        if (input.Get(neighbor) != '#')
        {
          stack.emplace(State2{neighbor, state.cost + 1, lastIntersection, lastIntersectionCost, visitedState});
        }
      }
    }

    State state;
    std::stack<State> stack2;
    stack2.emplace(State{Index2D{1, 0}, 0, std::set<Index2D>{Index2D{1, 0}}});
    while(!stack2.empty())
    {
      State state = stack2.top();
      stack2.pop();

      if (state.pos == Index2D{input.width - 2, input.height - 1})
      {
        if(max < state.cost)
          std::cout << "Found new max: " << state.cost << std::endl;
        max = std::max(max, state.cost);
        continue;
      }

      for (auto& [next, cost] : graph.GetPaths(state.pos))
      {
        std::set<Index2D> visited = state.visited;
        if (!visited.emplace(next).second)
          continue;
        stack2.emplace(State{next, state.cost + cost, visited});
      }
    }

    return max;
  }
}
