#pragma once

#include <map>
#include <set>

template <typename Key, typename Value>
class Graph
{
  std::map<Key, std::map<Key, Value>> graph;

  // Data based on the graph
  std::set<Key> nodes;

public:
  Graph() = default;

  void AddNode(const Key& from)
  {
    graph.emplace(from);
  }

  void AddOneWayEdge(const Key& from, const Key& to, const Value& value)
  {
    graph[from][to] = value;
    nodes.emplace(from);
  }

  void AddTwoWayEdge(const Key& from, const Key& to, const Value& value)
  {
    graph[from][to] = value;
    graph[to][from] = value;
    nodes.emplace(from);
    nodes.emplace(to);
  }

  Value DFSLong(const Key& from, const Key& to)
  {
    std::set<Key> visited;
    return DFSLong(to, visited, from, Value{});
  }

  Value DFS(const Key& from, const Key& to)
  {
    std::set<Key> visited;
    return DFS(to, visited, from, Value{});
  }

  const Value& GetEdge(const Key& from, const Key& to) const
  {
    // Verification that the nodes exists in the graph?
    return graph.find(from)->second.find(to)->second;
  }

  const std::map<Key, Value>& GetPaths(const Key& from) const
  {
    return graph.find(from)->second;
  }

  const std::set<Key>& GetNodes() const
  {
    return nodes;
  }

private:
  Value DFSLong(const Key& to, std::set<Key>& visited, const Key& current, const Value& cost)
  {
    if (visited.count(current))
      return Value{};
    if (current == to)
      return cost;

    visited.emplace(current);
    Value max{};
    for (auto& [next, nextCost] : graph.find(current)->second)
    {
      max = std::max(max, DFSLong(to, visited, next, cost + nextCost));
    }
    visited.erase(current);
    return max;
  }

  Value DFS(const Key& to, std::set<Key>& visited, const Key& current, const Value& cost)
  {
    if (visited.count(current))
      return Value{};
    if (current == to)
      return cost;

    visited.emplace(current);
    for (auto& [next, nextCost] : graph.find(current)->second)
    {
      Value val = DFS(to, visited, next, cost + nextCost);
      if (val != Value{})
        return val;
    }
    visited.erase(current);
    return Value{};
  }
};