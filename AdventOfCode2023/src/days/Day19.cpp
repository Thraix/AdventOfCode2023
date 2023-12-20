#include "aoc.h"

namespace day19
{
  struct Rule
  {
    char c;
    bool lt;
    int i;
    std::string res;
  };

  struct Rules
  {
    std::vector<Rule> rules;
    std::string otherwise;
  };

  struct Instructions
  {
    std::map<std::string, Rules> rules;
    std::vector<std::map<char, int>> data;
  };

  REGISTER_DAY(day19, Instructions, int64_t);

  REGISTER_TEST_EXAMPLE(day19, ExampleInput, 1, 19114);
  REGISTER_TEST        (day19, Input,        1, 532551);
  REGISTER_TEST_EXAMPLE(day19, ExampleInput, 2, 167409079868000);
  REGISTER_TEST        (day19, Input,        2, 134343280273968);

  READ_INPUT(input)
  {
    std::string str;
    Instructions instructions;
    while (std::getline(input, str))
    {
      if (str.empty())
        break;
      std::stringstream ss{str};
      std::string state;
      std::getline(ss, state, '{');
      char c;
      Rules rules;
      while (std::getline(ss, str, ','))
      {
        if (str.back() == '}')
        {
          str.pop_back();
          rules.otherwise = str;
        }
        else
        {
          std::stringstream ss{str};
          Rule rule;
          char lt;
          ss >> rule.c >> lt >> rule.i >> ":" >> rule.res;
          rule.lt = lt == '<';
          rules.rules.emplace_back(rule);
        }
      }
      instructions.rules.emplace(state, rules);
    }

    while (std::getline(input, str))
    {
      std::stringstream ss{str};
      int x, m, a, s;
      ss >> "{x=" >> x >> ",m=" >> m >> ",a=" >> a >> ",s=" >> s;
      instructions.data.emplace_back(std::map<char, int>{{'x', x}, {'m', m}, {'a', a}, {'s', s}});
    }

    return instructions;
  }

  bool Accept(const Instructions& ins, const std::map<char, int>& data, const std::string& workflow)
  {
    if (workflow == "A")
      return true;
    if (workflow == "R")
      return false;
    const Rules& rules = ins.rules.at(workflow);
    for (auto& rule : rules.rules)
    {
      int i = data.at(rule.c);
      if (rule.lt)
      {
        if (i < rule.i)
          return Accept(ins, data, rule.res);
      }
      else
      {
        if (i > rule.i)
          return Accept(ins, data, rule.res);
      }
    }

    return Accept(ins, data, rules.otherwise);
  }

  int64_t Accept2(const Instructions& ins, std::map<char, std::pair<int, int>> data, const std::string& workflow)
  {
    if (workflow == "A")
    {
      return (int64_t)(data['x'].second - data['x'].first + 1) *
             (int64_t)(data['m'].second - data['m'].first + 1) *
             (int64_t)(data['a'].second - data['a'].first + 1) *
             (int64_t)(data['s'].second - data['s'].first + 1);
    }
    if (workflow == "R")
      return 0;

    int64_t sum = 0;
    const Rules& rules = ins.rules.at(workflow);
    for (auto& rule : rules.rules)
    {
      if (rule.lt)
      {
        std::map<char, std::pair<int, int>> newData = data;
        newData[rule.c].second = rule.i - 1;
        sum += Accept2(ins, newData, rule.res);
        data[rule.c].first = rule.i;
      }
      else
      {
        std::map<char, std::pair<int, int>> newData = data;
        newData[rule.c].first = rule.i + 1;
        sum += Accept2(ins, newData, rule.res);
        data[rule.c].second = rule.i;
      }
    }

    return sum + Accept2(ins, data, rules.otherwise);
  }

  OUTPUT1(input)
  {
    int sum = 0;
    for (auto& data : input.data)
    {
      if (Accept(input, data, "in"))
      {
        sum += data.at('x') + data.at('m') + data.at('a') + data.at('s');
      }
    }
    return sum;
  }

  OUTPUT2(input)
  {
    return Accept2(input, {{'x', {1, 4000}}, {'m', {1, 4000}}, {'a', {1, 4000}}, {'s', {1, 4000}}}, "in");
  }
}
