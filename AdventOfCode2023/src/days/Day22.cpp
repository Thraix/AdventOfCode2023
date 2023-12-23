#include "aoc.h"

namespace day22
{
  struct Brick
  {
    Index3D pos;
    Index3D pos2;
    int index;

    friend std::ostream& operator<<(std::ostream& stream, const Brick& brick)
    {
      return stream << brick.index << ": " << brick.pos << " - " << brick.pos2;
    }
  };
  REGISTER_DAY(day22, std::vector<Brick>, int);

  REGISTER_TEST_EXAMPLE(day22, ExampleInput, 1, 5);
  REGISTER_TEST        (day22, Input,        1, 517);
  REGISTER_TEST_EXAMPLE(day22, ExampleInput, 2, 7);
  REGISTER_TEST        (day22, Input,        2, 61276);

  READ_INPUT(input)
  {
    std::string str;
    std::vector<Brick> bricks;
    int i = 0;
    while (std::getline(input, str))
    {
      std::stringstream ss{str};
      Brick brick;
      ss >> brick.pos.x >> "," >> brick.pos.y >> "," >> brick.pos.z >> "~" >> brick.pos2.x >> "," >> brick.pos2.y >> "," >> brick.pos2.z;

      // Make it so that pos is always the minimum position and pos2 is always the maximum position
      Brick actualBrick;
      actualBrick.pos.x = std::min(brick.pos.x, brick.pos2.x);
      actualBrick.pos.y = std::min(brick.pos.y, brick.pos2.y);
      actualBrick.pos.z = std::min(brick.pos.z, brick.pos2.z);
      actualBrick.pos2.x = std::max(brick.pos.x, brick.pos2.x);
      actualBrick.pos2.y = std::max(brick.pos.y, brick.pos2.y);
      actualBrick.pos2.z = std::max(brick.pos.z, brick.pos2.z);
      actualBrick.index = i;
      i++;
      bricks.emplace_back(actualBrick);
    }
    return bricks;
  }

  int GetZ(const Brick& brick, const std::map<Index3D, int>& occupied)
  {
    int z = brick.pos.z;
    while (z > 1)
    {
      for (int y = brick.pos.y; y <= brick.pos2.y; y++)
      {
        for (int x = brick.pos.x; x <= brick.pos2.x; x++)
        {
          auto it = occupied.find(Index3D{x, y, z - 1});
          if (it != occupied.end())
          {
            return z;
          }
        }
      }
      z--;
    }
    return z;
  }

  bool CanMove(const Brick& brick, const std::map<Index3D, int>& occupied, int ignore)
  {
    if (brick.pos.z == 1)
      return false;
    for (int y = brick.pos.y; y <= brick.pos2.y; y++)
    {
      for (int x = brick.pos.x; x <= brick.pos2.x; x++)
      {
        auto it = occupied.find(Index3D{x, y, brick.pos.z - 1});
        if (it != occupied.end() && it->second != brick.index && it->second != ignore)
        {
          return false;
        }
      }
    }
    return true;
  }

  void Set(const Brick& brick, std::map<Index3D, int>& occupied)
  {
    for (int z = brick.pos.z; z <= brick.pos2.z; z++)
    {
      for (int y = brick.pos.y; y <= brick.pos2.y; y++)
      {
        for (int x = brick.pos.x; x <= brick.pos2.x; x++)
        {
          occupied.emplace(Index3D{x, y, z}, brick.index);
        }
      }
    }
  }

  bool CanRemoveBrick(int brickIndex, const std::vector<Brick>& bricks, const std::map<Index3D, int>& occupied)
  {
    for (int i = 0; i < bricks.size(); i++)
    {
      if (CanMove(bricks[i], occupied, brickIndex))
      {
        return false;
      }
    }
    return true;
  }

  void Settle(std::vector<Brick>& bricks, std::map<Index3D, int>& occupied)
  {
    std::sort(bricks.begin(), bricks.end(), [](const Brick& lhs, const Brick& rhs) { return lhs.pos.z < rhs.pos.z; });
    for (int i = 0; i < bricks.size(); i++)
    {
      int z = GetZ(bricks[i], occupied);
      bricks[i].pos2.z = z + (bricks[i].pos2.z - bricks[i].pos.z);
      bricks[i].pos.z = z;
      Set(bricks[i], occupied);
    }
    std::sort(bricks.begin(), bricks.end(), [](const Brick& lhs, const Brick& rhs) { return lhs.pos.z < rhs.pos.z; });
  }

  OUTPUT1(input)
  {
    std::map<Index3D, int> occupied;
    std::vector<Brick> cpy = input;
    Settle(cpy, occupied);
    int sum = 0;
    for(int i = 0; i < input.size(); i++)
    {
      if (CanRemoveBrick(input[i].index, cpy, occupied))
        sum++;
    }
    return sum;
  }

  OUTPUT2(input)
  {
    std::vector<Brick> cpy = input;
    std::map<Index3D, int> occupied;
    Settle(cpy, occupied);

    int sum = 0;
    for (int i = 0; i < cpy.size(); i++)
    {
      std::map<Index3D, int> occupied;
      std::set<Brick> handled;
      std::vector<Brick> cpy2 = cpy;
      for (int j = 0; j < cpy.size(); j++)
      {
        if (j == i)
          continue;
        int z = GetZ(cpy2[j], occupied);
        if (z != cpy2[j].pos.z)
          sum++;
        cpy2[j].pos2.z = z + (cpy[j].pos2.z - cpy[j].pos.z);
        cpy2[j].pos.z = z;
        Set(cpy2[j], occupied);
      }
    }
    return sum;
  }
}
