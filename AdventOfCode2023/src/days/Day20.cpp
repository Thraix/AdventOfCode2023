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
  REGISTER_DAY(day20, Gates, int);

  REGISTER_TEST_EXAMPLE(day20, ExampleInput, 1, 0);
  REGISTER_TEST        (day20, Input,        1, 0);
  REGISTER_TEST_EXAMPLE(day20, ExampleInput, 2, 0);
  REGISTER_TEST        (day20, Input,        2, 0);

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
    std::map<std::string, bool> flipFlopStates;
    std::map<std::string, std::map<std::string, bool>> conjunction;
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
        conjunction.emplace(gate.first.name, states);
      }
    }
    std::pair<int, int> sigs{0, 0};
    for (int i = 0; i < 1000; i++)
    {
      std::pair<int, int> signals = SendSignal(input, "roadcaster", "", false, flipFlopStates, conjunction);
      sigs.first += signals.first;
      sigs.second += signals.second;
    }
    return (int64_t)sigs.first * (int64_t)sigs.second;
  }

  OUTPUT2(input)
  {
    return 0;
  }
}
