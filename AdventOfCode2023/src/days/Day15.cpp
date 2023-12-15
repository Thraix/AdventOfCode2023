#include "aoc.h"

namespace day15
{
  REGISTER_DAY(day15, std::vector<std::string>, int);

  REGISTER_TEST_EXAMPLE(day15, ExampleInput, 1, 1320);
  REGISTER_TEST        (day15, Input,        1, 497373);
  REGISTER_TEST_EXAMPLE(day15, ExampleInput, 2, 145);
  REGISTER_TEST        (day15, Input,        2, 259356);

  READ_INPUT(input)
  {
    std::string str;
    std::vector<std::string> strs;
    while (std::getline(input, str, ','))
    {
      strs.emplace_back(str);
    }
    return strs;
  }

  OUTPUT1(input)
  {
    int sum = 0;
    for (auto& str : input)
    {
      int code = 0;
      for(auto c : str)
      {
        code += c;
        code *= 17;
        code %= 256;
      }
      sum += code;
    }
    return sum;
  }

  OUTPUT2(input)
  {
    std::vector<std::vector<std::pair<std::string, int>>> boxes;
    boxes.resize(256);

    for (auto& str : input)
    {
      int boxNr = 0;
      int index = 0;
      for(auto c : str)
      {
        if (c == '=' || c == '-')
          break;

        boxNr += c;
        boxNr *= 17;
        boxNr %= 256;
        index++;
      }

      std::string box = str.substr(0, index);
      auto it = std::find_if(boxes[boxNr].begin(), boxes[boxNr].end(), [&](const std::pair<std::string, int>& elem)
        {
          return elem.first == box;
        });

      if (str[index] == '=')
      {
        int focalLength = std::stoi(str.substr(index + 1));
        if (it != boxes[boxNr].end())
          it->second = focalLength;
        else
          boxes[boxNr].emplace_back(std::make_pair(box, focalLength));
      }
      else
      {
        if (it != boxes[boxNr].end())
          boxes[boxNr].erase(it);
      }
    }
    int sum = 0;
    for (int i = 0; i < boxes.size(); i++)
    {
      for (int j = 0; j < boxes[i].size(); j++)
      {
        sum += (i + 1) * (j + 1) * boxes[i][j].second;
      }
    }
    return sum;
  }
}
