#include "aoc.h"

namespace day14
{
  REGISTER_DAY(day14, Array2D<char>, int);

  REGISTER_TEST_EXAMPLE(day14, ExampleInput, 1, 136);
  REGISTER_TEST        (day14, Input,        1, 113525);
  REGISTER_TEST_EXAMPLE(day14, ExampleInput, 2, 64);
  REGISTER_TEST        (day14, Input,        2, 101292);

  READ_INPUT(input)
  {
    return Input::ReadArray2D(input);
  }

  void North(Array2D<char>& map)
  {
    for (int y = 0; y < map.height; y++)
    {
      for (int x = 0; x < map.width; x++)
      {
        if (map.Get(x, y) == 'O')
        {
          map.Set(x, y, '.');
          int i = y;
          while (i >= 0 && map.Get(x, i) == '.')
          {
            i--;
          }
          i++;
          map.Set(x, i, 'O');
        }
      }
    }
  }

  void South(Array2D<char>& map)
  {
    for (int y = map.height - 1; y >= 0; y--)
    {
      for (int x = 0; x < map.width; x++)
      {
        if (map.Get(x, y) == 'O')
        {
          map.Set(x, y, '.');
          int i = y;
          while (i < map.height && map.Get(x, i) == '.')
          {
            i++;
          }
          i--;
          map.Set(x, i, 'O');
        }
      }
    }
  }

  void West(Array2D<char>& map)
  {
    for (int y = 0; y < map.height; y++)
    {
      for (int x = 0; x < map.width; x++)
      {
        if (map.Get(x, y) == 'O')
        {
          map.Set(x, y, '.');
          int i = x;
          while (i >= 0 && map.Get(i, y) == '.')
          {
            i--;
          }
          i++;
          map.Set(i, y, 'O');
        }
      }
    }
  }

  void East(Array2D<char>& map)
  {
    for (int y = 0; y < map.height; y++)
    {
      for (int x = map.width - 1; x >= 0; x--)
      {
        if (map.Get(x, y) == 'O')
        {
          map.Set(x, y, '.');
          int i = x;
          while (i < map.width && map.Get(i, y) == '.')
          {
            i++;
          }
          i--;
          map.Set(i, y, 'O');
        }
      }
    }
  }

  int CalculateLoad(const Array2D<char>& map)
  {
    int load = 0;
    for (int y = 0; y < map.height; y++)
    {
      for (int x = 0; x < map.width; x++)
      {
        if(map.Get(x, y) == 'O')
          load += (map.height - y);
      }
    }
    return load;
  }

  OUTPUT1(input)
  {
    Array2D<char> map = input;
    North(map);
    return CalculateLoad(map);
  }

  OUTPUT2(input)
  {
    Array2D<char> map = input;
    std::vector<int> loads;
    for (int i = 0; i < 1000000000; i++)
    {
      North(map);
      West(map);
      South(map);
      East(map);
      int load = CalculateLoad(map);
      loads.emplace_back(load);

      for (int i = loads.size() - 2; i >= 0; i--)
      {
        if (loads[i] == load)
        {
          int loopSize = loads.size() - i - 1;
          bool hasLoop = true;
          for (int j = 0; j < loopSize; j++)
          {
            int k = loads.size() - j - 1;
            // Check if the same load has occured in 3 loops.
            if (k - loopSize * 3 < 0 || loads[k - loopSize * 3] != loads[k] ||
                                        loads[k - loopSize * 2] != loads[k] ||
                                        loads[k - loopSize * 1] != loads[k])
            {
              hasLoop = false;
            }
          }
          if (hasLoop)
          {
            int loopOffset = (1000000000 - loads.size()) % loopSize;
            return loads[loads.size() - 1 - loopSize + loopOffset];
          }
        }
      }
    }
    return 0;
  }
}
