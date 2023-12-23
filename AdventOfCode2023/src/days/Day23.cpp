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

  struct DP
  {
    std::set<Index2D> visited;
    Index2D pos;

    bool operator<(const DP& rhs) const
    {
      if (pos != rhs.pos)
        return pos < rhs.pos;
      if (visited.size() != rhs.visited.size())
        return visited.size() < rhs.visited.size();

      std::set<Index2D>::iterator it1 = visited.begin();
      std::set<Index2D>::iterator it2 = rhs.visited.begin();
      for (int i = 0; i < visited.size(); i++)
      {
        if (*it1 != *it2)
          return *it1 < *it2;
        it1++;
        it2++;
      }
      return false;
    }
  };

  std::set<Index2D> GetIntersections(const Array2D<char>& map)
  {
    std::set<Index2D> intersections;
    for (int y = 0; y < map.height; y++)
    {
      for (int x = 0; x < map.width; x++)
      {
        if (map.Get(x, y) != '.')
          continue;

        int paths = 0;
        for (auto& neighbor : map.GetNeighbors(Index2D{x, y}, false))
        {
          if(map.Get(neighbor) != '#')
            paths++;
        }
        if (paths != 2)
          intersections.emplace(Index2D{x, y});
      }
    }
    return intersections;
  }

  void FindNextIntersection1(const Array2D<char>& map, Graph<Index2D, int>& graph, Index2D from, const std::set<Index2D>& intersections)
  {
    for (auto& neighbor : map.GetNeighbors(from, false))
    {
      if (map.Get(neighbor) == '#')
        continue;

      std::set<Index2D> visited;
      visited.emplace(from);
      visited.emplace(neighbor);

      Index2D pos = neighbor;
      int cost = 1;
      bool canFind = true;
      while (canFind && intersections.count(pos) == 0)
      {
        canFind = false;
        for (auto& next : map.GetNeighbors(pos, false))
        {
          if(!visited.emplace(next).second)
            continue;
          if (map.Get(next) == '.' ||
             map.Get(next) == '<' && next.x < pos.x ||
             map.Get(next) == '>' && next.x > pos.x ||
             map.Get(next) == 'v' && next.y > pos.y ||
             map.Get(next) == '^' && next.y < pos.y)
          {
            pos = next;
            canFind = true;
            break;
          }
        }
        cost++;
      }
      if(canFind)
        graph.AddOneWayEdge(from, pos, cost);
    }
  }

  void FindNextIntersection2(const Array2D<char>& map, Graph<Index2D, int>& graph, Index2D from, const std::set<Index2D>& intersections)
  {
    for (auto& neighbor : map.GetNeighbors(from, false))
    {
      if (map.Get(neighbor) == '#')
        continue;

      std::set<Index2D> visited;
      visited.emplace(from);
      visited.emplace(neighbor);

      Index2D pos = neighbor;
      int cost = 1;
      while (intersections.count(pos) == 0)
      {
        for (auto& neighbor : map.GetNeighbors(pos, false))
        {
          if (map.Get(neighbor) != '#' && visited.emplace(neighbor).second)
          {
            pos = neighbor;
            break;
          }
        }
        cost++;
      }
      graph.AddOneWayEdge(from, pos, cost);
    }
  }

  OUTPUT1(input)
  {
    Graph<Index2D, int> graph;
    std::set<Index2D> intersections = GetIntersections(input);
    for (auto& intersection : intersections)
    {
      FindNextIntersection1(input, graph, intersection, intersections);
    }
    return graph.DFSLong(Index2D{1, 0}, Index2D{input.width - 2, input.height - 1});
  }

  OUTPUT2(input)
  {
    Graph<Index2D, int> graph;
    std::set<Index2D> intersections = GetIntersections(input);

    for (auto& intersection : intersections)
    {
      FindNextIntersection2(input, graph, intersection, intersections);
    }
    return graph.DFSLong(Index2D{1, 0}, Index2D{input.width - 2, input.height - 1});
  }
}
