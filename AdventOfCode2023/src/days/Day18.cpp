#include "aoc.h"

namespace day18
{
  struct Dig
  {
    char dir;
    int amount;
    std::string color;
  };

  REGISTER_DAY(day18, std::vector<Dig>, int64_t);

  REGISTER_TEST_EXAMPLE(day18, ExampleInput, 1, 62);
  REGISTER_TEST        (day18, Input,        1, 35244);
  REGISTER_TEST_EXAMPLE(day18, ExampleInput, 2, 952408144115);
  REGISTER_TEST        (day18, Input,        2, 85070763635666);

  READ_INPUT(input)
  {
    std::string str;
    std::vector<Dig> digs;
    while (std::getline(input, str))
    {
      Dig dig;
      std::stringstream ss{str};
      ss >> dig.dir >> dig.amount >> "(#" >> dig.color;
      dig.color.pop_back();
      digs.emplace_back(dig);
    }
    return digs;
  }

  int64_t Area(const std::vector<Dig>& digs)
  {
    Index2D pos{0, 0};
    std::vector<Index2D> indices;
    for (auto& dig : digs)
    {
      if (dig.dir == 'R')
        pos.x += dig.amount;
      if (dig.dir == 'L')
        pos.x -= dig.amount;
      if (dig.dir == 'D')
        pos.y += dig.amount;
      if (dig.dir == 'U')
        pos.y -= dig.amount;
      indices.emplace_back(pos);
    }

    int64_t area = 0;
    for (int i = 0; i < digs.size() - 1; i++)
    {
      area += ((int64_t)indices[i].x * (int64_t)indices[i + 1].y) - ((int64_t)indices[i].y * (int64_t)indices[i + 1].x);
    }

    area = std::abs(area / 2);
    for (auto& dig : digs)
    {
      if (dig.dir == 'R')
        area += dig.amount;
      if (dig.dir == 'U')
        area += dig.amount;
    }
    return area + 1;
  }

  int HexToDec(const std::string& str)
  {
    int sum = 0;
    for (char c : str)
    {
      if(Helper::IsDigit(c))
        sum = sum * 16 + (c - '0');
      else
        sum = sum * 16 + (c - 'a' + 10);
    }
    return sum;
  }

  OUTPUT1(input)
  {
    return Area(input);
  }

  OUTPUT2(input)
  {
    std::vector<Dig> digs = input;
    for (auto& dig : digs)
    {
      int dec = HexToDec(dig.color);
      dig.amount = dec / 16;
      int dir = dec % 4;
      if (dir == 0) dig.dir = 'R';
      if (dir == 1) dig.dir = 'D';
      if (dir == 2) dig.dir = 'L';
      if (dir == 3) dig.dir = 'U';
    }
    return Area(digs);
  }
}
