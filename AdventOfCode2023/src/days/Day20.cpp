#include "aoc.h"

namespace day20
{
  struct Gate
  {
    char op;
    std::string name;

    bool operator<(const Gate& rhs) const
    {
      return name < rhs.name;
    }
  };

  using Gates = std::map<Gate, std::vector<std::string>>;

  REGISTER_DAY(day20, Gates, int64_t);

  REGISTER_TEST_EXAMPLE(day20, ExampleInput, 1, 11687500);
  REGISTER_TEST        (day20, Input,        1, 743090292);
  // REGISTER_TEST_EXAMPLE(day20, ExampleInput, 2, 0);
  REGISTER_TEST        (day20, Input,        2, 241528184647003);

  READ_INPUT(input)
  {
    std::string str;
    std::map<Gate, std::vector<std::string>> gates;
    while (std::getline(input, str))
    {
      std::stringstream ss{str};
      Gate gate;
      ss >> gate.op >> gate.name >> "->";
      std::vector<std::string> target;
      while (std::getline(ss, str, ','))
      {
        target.emplace_back(str.substr(1));
      }
      gates.emplace(gate, target);
    }
    return gates;
  }

  struct State
  {
    std::map<std::string, bool> flipFlopStates;
    std::map<std::string, std::map<std::string, bool>> conjunctionStates;
  };

  struct Signal
  {
    std::string from;
    std::string to;
    bool high;
  };

  State InitiateState(const Gates& gates)
  {
    State state;
    for (auto& gate : gates)
    {
      std::map<std::string, bool> states;
      if (gate.first.op == '&')
      {
        for (auto& other : gates)
        {
          for (auto& target : other.second)
          {
            if (target == gate.first.name)
            {
              states.emplace(other.first.name, false);
            }
          }
        }
        state.conjunctionStates.emplace(gate.first.name, states);
      }
    }
    return state;
  }

  void HandleFlipFlop(const Signal& signal, const std::vector<std::string>& targets, std::queue<Signal>& queue, State& state)
  {
    if (!signal.high)
    {
      state.flipFlopStates[signal.to] = !state.flipFlopStates[signal.to];
      for (auto& target : targets)
      {
        queue.emplace(Signal{signal.to, target, state.flipFlopStates[signal.to]});
      }
    }
  }

  bool HandleConjunction(const Signal& signal, const std::vector<std::string>& targets, std::queue<Signal>& queue, State& state)
  {
    state.conjunctionStates[signal.to][signal.from] = signal.high;
    bool sendHigh = false;
    for (auto& states : state.conjunctionStates[signal.to])
    {
      if (!states.second)
      {
        sendHigh = true;
        break;
      }
    }
    for (auto& target : targets)
    {
      queue.emplace(Signal{signal.to, target, sendHigh});
    }
    return sendHigh;
  }

  void HandleBroadcaster(const Signal& signal, const std::vector<std::string>& targets, std::queue<Signal>& queue)
  {
    for (auto& target : targets)
    {
      queue.emplace(Signal{signal.to, target, signal.high});
    }
  }

  int64_t CalculateCommonStart(int64_t start1, int64_t loop1, int64_t start2, int64_t loop2)
  {
    while (start1 != start2)
    {
      if (start1 < start2)
      {
        int count = std::max(1ll, (start2 - start1) / loop1);
        start1 += count * loop1;
      }
      else
      {
        int count = std::max(1ll, (start1 - start2) / loop2);
        start2 += count * loop2;
      }
    }
    return start1;
  }

  int64_t CalculateSolution(const std::vector<std::pair<int, int>>& startLoops)
  {
    int64_t start1 = startLoops[0].first;
    int64_t loop1 = startLoops[0].second;
    for (int i = 1; i < startLoops.size(); i++)
    {
      start1 = CalculateCommonStart(start1, loop1, startLoops[i].first, startLoops[i].second);
      loop1 = std::lcm(loop1, startLoops[i].second);
    }
    return start1;
  }

  bool IsDone(const std::map<std::string, std::pair<int, int>>& gateLoops, const std::set<std::string>& neededGates)
  {
    if (gateLoops.size() != neededGates.size())
      return false;
    for (auto& loop : gateLoops)
    {
      if (loop.second.second == 0) return false;
    }
    return true;
  }

  std::set<std::string> GetGatesToTarget(const Gates& gates, const std::string& target)
  {
    std::set<std::string> sources;
    for (auto& gate : gates)
    {
      for (auto& t : gate.second)
      {
        if (t == target)
          sources.emplace(gate.first.name);
      }
    }
    return sources;
  }

  OUTPUT1(input)
  {
    State state = InitiateState(input);

    std::pair<int, int> signals{0, 0};
    for (int i = 0; i < 1000; i++)
    {
      std::queue<Signal> queue;
      queue.emplace(Signal{"", "roadcaster", false});
      while (!queue.empty())
      {
        Signal signal = queue.front();
        queue.pop();
        if (signal.high)
          signals.first += 1;
        else
          signals.second += 1;
        auto it = input.find(Gate{'x', signal.to});
        if (it == input.end())
          continue;
        if (it->first.op == '%')
          HandleFlipFlop(signal, it->second, queue, state);
        else if (it->first.op == '&')
          HandleConjunction(signal, it->second, queue, state);
        else if (it->first.name == "roadcaster")
          HandleBroadcaster(signal, it->second, queue);
      }
    }
    return (int64_t)signals.first * (int64_t)signals.second;
  }

  OUTPUT2(input)
  {
    State state = InitiateState(input);

    std::set<std::string> gateToRx = GetGatesToTarget(input, "rx"); // will only be 1 source gate
    std::set<std::string> neededGates = GetGatesToTarget(input, *gateToRx.begin());

    std::map<std::string, std::pair<int, int>> loops;
    int i = 0;
    while(true)
    {
      i++;
      std::queue<Signal> queue;
      queue.emplace(Signal{"", "roadcaster", false});
      while (!queue.empty())
      {
        Signal signal = queue.front();
        queue.pop();
        auto it = input.find(Gate{'x', signal.to});
        if (it == input.end())
          continue;
        if (it->first.op == '%')
        {
          HandleFlipFlop(signal, it->second, queue, state);
        }
        else if (it->first.op == '&')
        {
          if (HandleConjunction(signal, it->second, queue, state))
          {
            if (neededGates.count(signal.to))
            {
              int loopSize = 0;
              auto it = loops.find(signal.to);
              if (it != loops.end())
              {
                loopSize = (i - it->second.first);
              }
              loops[signal.to] = {i, loopSize};
            }
          }
        }
        else if (it->first.name == "roadcaster")
          HandleBroadcaster(signal, it->second, queue);
      }
      if (IsDone(loops, neededGates))
        break;
    }
    std::vector<std::pair<int, int>> result;
    for (auto& loop : loops)
    {
      result.emplace_back(loop.second);
    }
    return CalculateSolution(result);
  }
}
