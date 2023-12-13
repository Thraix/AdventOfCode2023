#include "aoc.h"

namespace day13
{
  REGISTER_DAY(day13, std::vector<Array2D<char>>, int);

  REGISTER_TEST_EXAMPLE(day13, ExampleInput, 1, 405);
  REGISTER_TEST        (day13, Input,        1, 36448);
  REGISTER_TEST_EXAMPLE(day13, ExampleInput, 2, 400);
  REGISTER_TEST        (day13, Input,        2, 35799);

  READ_INPUT(input)
  {
    std::string str;
    std::vector<Array2D<char>> maps;

    Array2D<char> array = Input::ReadArray2D(input);
    while (array.width != 0)
    {
      maps.emplace_back(array);
      array = Input::ReadArray2D(input);
    }
    return maps;
  }

  bool IsRowRecleted(const Array2D<char>& map, int x, int y)
  {
    for (int i = 0; i < map.width; i++)
    {
      if (!map.IsInside(x + i + 1, y) || !map.IsInside(x - i, y))
        break;
      if (map.Get(x + i + 1, y) != map.Get(x - i, y))
        return false;
    }
    return true;
  }

  bool IsColumnRecleted(const Array2D<char>& map, int x, int y)
  {
    for (int i = 0; i < map.height; i++)
    {
      if (!map.IsInside(x, y + i + 1) || !map.IsInside(x, y - i))
        break;
      if (map.Get(x, y + i + 1) != map.Get(x, y - i))
        return false;
    }
    return true;
  }

  bool IsReflectedVertical(const Array2D<char>& map, int x)
  {
    for(int i = 0; i < map.height; i++)
    {
      if (!IsRowRecleted(map, x, i))
        return false;
    }
    return true;
  }

  bool IsReflectedHorizontal(const Array2D<char>& map, int y)
  {
    for(int i = 0; i < map.width; i++)
    {
      if (!IsColumnRecleted(map, i, y))
        return false;
    }
    return true;
  }

  Index2D GetReflection(const Array2D<char>& map, Index2D notIndex)
  {
    for (int x = 0; x < map.width - 1; x++)
    {
      if (IsReflectedVertical(map, x) && Index2D{x + 1, 0} != notIndex)
      {
        return Index2D{x + 1, 0};
      }
    }
    for (int y = 0; y < map.height - 1; y++)
    {
      if (IsReflectedHorizontal(map, y) && Index2D{0, y + 1} != notIndex)
      {
        return Index2D{0, y + 1};
      }
    }
    return Index2D{0, 0};
  }
 
  Index2D GetDifferentReflection(Array2D<char> map)
  {
    Index2D reflection = GetReflection(map, Index2D{0, 0});

    for (int y = 0; y < map.height; y++)
    {
      for (int x = 0; x < map.width; x++)
      {
        char c = map.Get(x, y);
        if (map.Get(x, y) == '#')
          map.Set(x, y, '.');
        else
          map.Set(x, y, '#');


        Index2D reflection2 = GetReflection(map, reflection);
        if (reflection2 != Index2D{0, 0} && reflection2 != reflection)
          return reflection2;

        map.Set(x, y, c);
      }
    }
    std::cout << "here" << std::endl;
  }

  // Too low: 35441
  OUTPUT1(input)
  {
    int i = 0;
    int64_t sum = 0;
    for (auto& array : input)
    {
      Index2D reflection = GetReflection(array, Index2D{0, 0});
      sum += reflection.x + reflection.y * 100;
    }
    return sum;
  }

  OUTPUT2(input)
  {
    int i = 0;
    int64_t sum = 0;
    for (auto& array : input)
    {
      Index2D reflection = GetDifferentReflection(array);
      sum += reflection.x + reflection.y * 100;
    }
    return sum;
  }
}
