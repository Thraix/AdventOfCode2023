#include "aoc.h"

namespace day20
{
  struct Gate
  {
    char op;
    std::string name;

    bool flipFlopState = false;
    std::map<std::string, bool> conjunctionState;

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

    bool operator<(const State& rhs) const
    {
      if(flipFlopStates != rhs.flipFlopStates)
        return flipFlopStates < rhs.flipFlopStates;
      return conjunctionStates < rhs.conjunctionStates;
    }
  };

  struct Signal
  {
    std::string from;
    std::string to;
    bool high;
  };

  std::pair<int, int> SendSignal(const std::map<Gate, std::vector<std::string>>& gates, const std::string& gate, const std::string& sender, bool high, std::map<std::string, bool>& flipFlopStates, std::map<std::string, std::map<std::string, bool>>& conjunctionStates)
  {
    std::pair<int, int> signals;
    if (high)
      signals.first = 1;
    else
      signals.second = 1;
    auto it = gates.find(Gate{'x', gate});
    if (it == gates.end())
      return signals;
    if (it->first.op == '%')
    {
      if (!high)
      {
        flipFlopStates[gate] = !flipFlopStates[gate];
        for (auto& target : it->second)
        {
          std::pair<int, int> sigs = SendSignal(gates, target, gate, flipFlopStates[gate], flipFlopStates, conjunctionStates);
          signals.first += sigs.first;
          signals.second += sigs.second;
        }
      }
    }
    else if (it->first.op == '&')
    {
      conjunctionStates[gate][sender] = high;
      bool valid = true;
      for (auto& states : conjunctionStates[gate])
      {
        if (!states.second)
        {
          valid = false;
          break;
        }
      }
      for (auto& target : it->second)
      {
        std::pair<int, int> sigs = SendSignal(gates, target, gate, !valid, flipFlopStates, conjunctionStates);
        signals.first += sigs.first;
        signals.second += sigs.second;
      }
    }
    else if (it->first.name == "roadcaster")
    {
      for (auto& target : it->second)
      {
        std::pair<int, int> sigs = SendSignal(gates, target, gate, high, flipFlopStates, conjunctionStates);
        signals.first += sigs.first;
        signals.second += sigs.second;
      }
    }

    return signals;
  }

  OUTPUT1(input)
  {
    State state;
    for (auto& gate : input)
    {
      std::map<std::string, bool> states;
      if (gate.first.op == '&')
      {
        for (auto& other : input)
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
        {
          if (!signal.high)
          {
            state.flipFlopStates[signal.to] = !state.flipFlopStates[signal.to];
            for (auto& target : it->second)
            {
              queue.emplace(Signal{signal.to, target, state.flipFlopStates[signal.to]});
            }
          }
        }
        else if (it->first.op == '&')
        {
          state.conjunctionStates[signal.to][signal.from] = signal.high;
          bool valid = true;
          for (auto& states : state.conjunctionStates[signal.to])
          {
            if (!states.second)
            {
              valid = false;
              break;
            }
          }
          for (auto& target : it->second)
          {
            queue.emplace(Signal{signal.to, target, !valid});
          }
        }
        else if (it->first.name == "roadcaster")
        {
          for (auto& target : it->second)
          {
            queue.emplace(Signal{signal.to, target, signal.high});
          }
        }
      }
    }
    return (int64_t)signals.first * (int64_t)signals.second;
  }

  int64_t FindCommon(int64_t start1, int64_t loop1, int64_t start2, int64_t loop2)
  {
    while (start1 != start2)
    {
      if (start1 < start2)
      {
        int count = std::ceil((start2 - start1) / (float)loop1);
        start1 += count * loop1;
      }
      else
      {
        int count = std::ceil((start1 - start2) / (float)loop2);
        start2 += count * loop2;
      }
    }
    return start1;
  }

  int64_t FindSolution(const std::vector<std::pair<int, int>>& startLoops)
  {
    int64_t start1 = startLoops[0].first;
    int64_t loop1 = startLoops[0].second;
    for (int i = 1; i < startLoops.size(); i++)
    {
      start1 = FindCommon(start1, loop1, startLoops[i].first, startLoops[i].second);
      loop1 = std::lcm(loop1, startLoops[i].second);
    }
    return start1;
  }

  bool IsDone(const std::map<std::string, std::pair<int, int>>& gates, const std::set<std::string>& neededGates)
  {
    if (gates.size() != neededGates.size())
      return false;
    for (auto& gate : gates)
    {
      if (gate.second.second == 0) return false;
    }
    return true;
  }

  std::set<std::string> GetGatesToTarget(const std::map<Gate, std::vector<std::string>>& gates, const std::string& target)
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


  OUTPUT2(input)
  {
    State state;
    for (auto& gate : input)
    {
      std::map<std::string, bool> states;
      if (gate.first.op == '&')
      {
        for (auto& other : input)
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

    std::set<std::string> gateToRx = GetGatesToTarget(input, "rx"); // will only be 1 source gate
    std::set<std::string> neededGates = GetGatesToTarget(input, *gateToRx.begin());

    std::map<std::string, std::pair<int, int>> timeSince1;
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
          if (!signal.high)
          {
            state.flipFlopStates[signal.to] = !state.flipFlopStates[signal.to];
            for (auto& target : it->second)
            {
              queue.emplace(Signal{signal.to, target, state.flipFlopStates[signal.to]});
            }
          }
        }
        else if (it->first.op == '&')
        {
          state.conjunctionStates[signal.to][signal.from] = signal.high;
          bool valid = true;
          for (auto& states : state.conjunctionStates[signal.to])
          {
            if (!states.second)
            {
              valid = false;
              break;
            }
          }
          if (!valid)
          {
            if (neededGates.count(signal.to))
            {
              int loop = 0;
              if (timeSince1.find(signal.to) != timeSince1.end())
              {
                loop = (i - timeSince1.find(signal.to)->second.first);
              }
              timeSince1[signal.to] = {i, loop};
            }
          }

          for (auto& target : it->second)
          {
            queue.emplace(Signal{signal.to, target, !valid});
          }
        }
        else if (it->first.name == "roadcaster")
        {
          for (auto& target : it->second)
          {
            queue.emplace(Signal{signal.to, target, signal.high});
          }
        }
      }
      if (IsDone(timeSince1, neededGates))
        break;
    }
    std::vector<std::pair<int, int>> result;
    for (auto& timeSince : timeSince1)
    {
      result.emplace_back(timeSince.second);
    }
    std::cout << result << std::endl;
    return FindSolution(result);
  }
}

// kv 272884 4013
// vb 273096 3793
// kl 274190 3917
// vm 275468 4051

// 272884 + 4013 * A = E
// 273096 + 3793 * B = E
// 274190 + 3917 * C = E
// 275468 + 4051 * D = E

// 272884 + 4013 * A = 273096 + 3793 * B
// 273096 + 3793 * B = E
