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

	const Value& GetEdge(const Key& from, const Key& to) const
	{
		// Verification that the nodes exists in the graph?
		return graph.find(from)->second.find(to)->second;
	}

	const std::set<Key>& GetNodes() const
	{
		return nodes;
	}
};