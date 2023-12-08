#include "aoc.h"

namespace day08
{
  struct Map
  {
    std::string instruction;
    std::string start;
    std::map<std::string, std::pair<std::string, std::string>> map;

  };
  REGISTER_DAY(day08, Map, int64_t);

  REGISTER_TEST_EXAMPLE(day08, ExampleInput, 1, 2);
  REGISTER_TEST        (day08, Input,        1, 19099);
  REGISTER_TEST_EXAMPLE(day08, ExampleInput, 2, 6);
  REGISTER_TEST        (day08, Input,        2, 17099847107071);

  READ_INPUT(input)
  {
    std::string str;
    std::vector<int> ints;
    Map map;
    std::getline(input, map.instruction);
    std::getline(input, str); // empty line
    while (std::getline(input, str))
    {
      std::string s1, s2, s3;
      std::stringstream ss{str};
      ss >> s1 >> "=" >> "(" >> s2 >> s3 >> ")";
      s2 = s2.substr(0, s2.size() - 1);
      s3 = s3.substr(0, s3.size() - 1);
      map.map.emplace(s1, std::make_pair(s2, s3));
      if (map.start.empty())
        map.start = s1;
    }
    return map;
  }

  OUTPUT1(input)
  {
    std::string currentPos = "AAA";
    int i = 0;
    while (currentPos != "ZZZ")
    {
      if(input.instruction[i % input.instruction.size()] == 'R')
        currentPos = input.map.at(currentPos).second;
      else
        currentPos = input.map.at(currentPos).first;
      i++;
    }
    return i;
  }

  OUTPUT2(input)
  {
    std::map<std::string, int64_t> steps;
    for(auto& [start, nodes] : input.map)
    {
      if (start[2] == 'A')
      {
        std::string currentPos = start;
        int i = 0;
        while (currentPos[2] != 'Z')
        {
          if(input.instruction[i % input.instruction.size()] == 'R')
            currentPos = input.map.at(currentPos).second;
          else
            currentPos = input.map.at(currentPos).first;
          i++;
        }
        steps[start] = i;
      }
    }
    int64_t lcm = 1;
    for (auto& [start, step] : steps)
    {
      lcm = std::lcm(lcm, step);
    }

    return lcm;
  }
}
