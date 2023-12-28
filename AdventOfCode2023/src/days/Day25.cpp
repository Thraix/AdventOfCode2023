#include "aoc.h"

namespace day25
{
  using Input = Graph<std::string, int>;
  REGISTER_DAY(day25, Input, int);

  REGISTER_TEST_EXAMPLE(day25, ExampleInput, 1, 54);
  REGISTER_TEST        (day25, Input,        1, 583632);
  REGISTER_TEST_EXAMPLE(day25, ExampleInput, 2, 0);
  REGISTER_TEST        (day25, Input,        2, 0);

  READ_INPUT(input)
  {
    std::string str;
    Graph<std::string, int> graph;
    while (std::getline(input, str))
    {
      std::stringstream ss{str};
      std::string s;
      ss >> s;
      s.pop_back();
      std::string s2;
      while (ss >> s2)
      {
        graph.AddTwoWayEdge(s, s2, 1);
      }
    }
    return graph;
  }

  std::pair<bool, int> FloodFill(const Graph<std::string, int>& graph, const std::set<std::pair<std::string, std::string>>& exclude)
  {
    std::set<std::string> visited;
    visited.emplace(*graph.GetNodes().begin());
    std::stack<std::string> stack;
    stack.emplace(*graph.GetNodes().begin());
    while (!stack.empty())
    {
      std::string s = stack.top();
      stack.pop();

      for (auto& [next, nextCost] : graph.GetPaths(s))
      {
        if (exclude.count(std::make_pair(next, s)) || exclude.count(std::make_pair(s, next)))
          continue;
        if (visited.emplace(next).second)
          stack.emplace(next);
      }
    }
    return {visited.size() != graph.GetNodes().size(), visited.size()};
  }

  OUTPUT1(input)
  {
    std::map<std::pair<std::string, std::string>, int> connectionCount;
    // Randomly select 2 sets of nodes and calculate the path between them.
    // Keep track of how many times each connection between two neighbor nodes are used
    for (int i = 0; i < 50; i++)
    {
      auto it1 = input.GetNodes().begin();
      auto it2 = input.GetNodes().begin();
      std::advance(it1, rand() % input.GetNodes().size());
      std::advance(it2, rand() % input.GetNodes().size());
      std::pair<int, std::vector<std::string>> paths = input.DijkstrasWithPath(*it1, *it2);
      for (int j = 0; j < paths.second.size() - 1; j++)
      {
        std::pair<std::string, std::string> connection;
        if (paths.second[j] < paths.second[j + 1])
          connection = std::make_pair(paths.second[j], paths.second[j + 1]);
        else
          connection = std::make_pair(paths.second[j + 1], paths.second[j]);
        connectionCount[connection]++;
      }
    }

    // Sort the connections based on number of uses, more uses likely means
    // it's one of the 3 connections connecting the two group of nodes
    std::vector<std::pair<std::string, std::string>> order;
    for (auto& connection : connectionCount) {
      order.emplace_back(connection.first);
    }
    std::sort(order.begin(), order.end(), [&](const auto& lhs, const auto& rhs) { return connectionCount[lhs] > connectionCount[rhs]; });

    // Go through the candidate connections starting with connections that were most visited.
    for (int i = 0; i < order.size(); i++)
    {
      for (int j = i + 1; j < order.size(); j++)
      {
        for (int k = j + 1; k < order.size(); k++)
        {
          auto [filled, count] = FloodFill(input, {order[i], order[j], order[k]});
          if (filled)
          {
            return count * (input.GetNodes().size() - count);
          }
        }
      }
    }

    // Unlucky ¯\_(*_*)_/¯
    return 0;
  }

  OUTPUT2(input)
  {
    std::cout << "  ___                      ___" << std::endl;
    std::cout << " (o o)                    (o o)" << std::endl;
    std::cout << "(  V  ) MERRY CHRISTMAS! (  V  )" << std::endl;
    std::cout << "--m-m----------------------m-m--" << std::endl;
    return 0;
  }
}
