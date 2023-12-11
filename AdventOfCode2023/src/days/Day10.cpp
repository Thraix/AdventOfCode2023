#include "aoc.h"

namespace day10
{
  REGISTER_DAY(day10, Array2D<char>, int);

  REGISTER_TEST_EXAMPLE(day10, ExampleInput, 1, 70);
  REGISTER_TEST        (day10, Input,        1, 6682);
  REGISTER_TEST_EXAMPLE(day10, ExampleInput, 2, 8);
  REGISTER_TEST        (day10, Input,        2, 353);

  READ_INPUT(input)
  {
    return Input::ReadArray2D(input);
  }

  enum class Direction
  {
    North, South, East, West
  };
  Index2D GetNextPosition(Index2D position, Direction direction)
  {
    switch (direction)
    {
    case Direction::North:
      return Index2D{position.x, position.y - 1};
    case Direction::South:
      return Index2D{position.x, position.y + 1};
    case Direction::East:
      return Index2D{position.x + 1, position.y};
    case Direction::West:
      return Index2D{position.x - 1, position.y};
    }
  }

  Direction GetNextDirection(const Array2D<char>& map, Index2D position, Direction prevDirection)
  {
    switch (prevDirection)
    {
      case Direction::North:
      {
        char c = map.Get(position.x, position.y - 1);
        if (c == '|')
          return Direction::North;
        else if (c == 'F')
          return Direction::East;
        else if (c == '7')
          return Direction::West;
      }
      case Direction::South:
      {
        char c = map.Get(position.x, position.y + 1);
        if (c == '|')
          return Direction::South;
        else if (c == 'L')
          return Direction::East;
        else if (c == 'J')
          return Direction::West;
      }
      case Direction::East:
      {
        char c = map.Get(position.x + 1, position.y);
        if (c == '-')
          return Direction::East;
        else if (c == 'J')
          return Direction::North;
        else if (c == '7')
          return Direction::South;
      }
      case Direction::West:
      {
        char c = map.Get(position.x - 1, position.y);
        if (c == '-')
          return Direction::West;
        else if (c == 'F')
          return Direction::South;
        else if (c == 'L')
          return Direction::North;
      }
    }
    std::cerr << "Invalid direction" << std::endl;
    return Direction::North;
  }

  std::vector<Index2D> GetPath(const Array2D<char>& map)
  {
    Index2D pos;
    for (int y = 0; y < map.height; y++)
    {
      for (int x = 0; x < map.width; x++)
      {
        if (map.Get(x, y) == 'S')
          pos = Index2D{x, y};
      }
    }

    std::set<char> south{'|', 'L', 'J'};
    std::set<char> north{'|', 'F', '7'};
    std::set<char> west{'-', 'F', 'L'};
    std::set<char> east{'-', 'J', '7'};

    std::vector<char> surrounding;
    Index2D cur;
    Direction nextDirection;
    if (pos.x > 0 && west.count(map.Get(pos.x - 1, pos.y)))
      nextDirection = Direction::West;
    else if (pos.y > 0 && north.count(map.Get(pos.x, pos.y - 1)))
      nextDirection = Direction::North;
    else if (pos.x < map.width - 1 && east.count(map.Get(pos.x + 1, pos.y)))
      nextDirection = Direction::East;
    else if (pos.y < map.height - 1 && south.count(map.Get(pos.x, pos.y + 1)))
      nextDirection = Direction::South;

    std::vector<Index2D> path;
    path.emplace_back(pos);
    while (map.Get(GetNextPosition(pos, nextDirection)) != 'S')
    {
      Index2D prevPos = pos;
      pos = GetNextPosition(pos, nextDirection);
      path.emplace_back(pos);
      nextDirection = GetNextDirection(map, prevPos, nextDirection);
    }
    return path;
  }

  void RemoveFill(Array2D<char>& map, Index2D pos)
  {
    std::stack<Index2D> stack;
    stack.emplace(pos);
    while (!stack.empty())
    {
      Index2D top = stack.top();
      stack.pop();
      if (map.Get(top) == '.')
      {
        map.Set(top.x, top.y, ' ');

        for (auto neighbor : map.GetNeighbors(top, false))
        {
          if (map.Get(neighbor) == '.')
            stack.emplace(neighbor);
        }
      }
    }
  }

  OUTPUT1(input)
  {
    return GetPath(input).size() / 2;
  }

  OUTPUT2(input)
  {

    std::vector<Index2D> path = GetPath(input);
    Array2D<char> newMap{input.width, input.height, '.'};

    for (int i = 0; i < path.size(); i++)
    {
      newMap.Set(path[i].x, path[i].y, input.Get(path[i]));
    }


    int sum = 0;
    for (int y = 0; y < newMap.height; y ++)
    {
      for (int x = 0; x < newMap.width; x++)
      {
        if (newMap.Get(x, y) == '.')
        {
          int add = 0;

          for (int i = 0; i < x; i++)
          {
            if (newMap.Get(i, y) == 'F') add++;
            if (newMap.Get(i, y) == 'J') add++;
            if (newMap.Get(i, y) == '7') add--;
            if (newMap.Get(i, y) == 'L') add--;
            if (newMap.Get(i, y) == '|') add += 2;
          }
          if (std::abs(add) / 2  % 2 == 1)
            sum++;

        }
      }
    }
    return sum;
  }
}
