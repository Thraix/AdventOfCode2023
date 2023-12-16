#include "aoc.h"

namespace day16
{
  struct Ray
  {
    Index2D pos;
    Index2D vel;
  };

  REGISTER_DAY(day16, Array2D<char>, int);

  REGISTER_TEST_EXAMPLE(day16, ExampleInput, 1, 46);
  REGISTER_TEST        (day16, Input,        1, 6855);
  REGISTER_TEST_EXAMPLE(day16, ExampleInput, 2, 51);
  REGISTER_TEST        (day16, Input,        2, 7513);

  int Energize(const Array2D<char>& map, const Ray& ray)
  {
    Array2D<char> energized{map.width, map.height, '.'};
    std::stack<Ray> rays;
    rays.emplace(ray);
    std::map<Index2D, bool> splitters;
    while (!rays.empty())
    {
      Ray ray = rays.top();
      rays.pop();
      while (true)
      {
        ray.pos = ray.pos + ray.vel;
        if (!map.IsInside(ray.pos))
          break;
        energized.Set(ray.pos.x, ray.pos.y, '#');

        if (map.Get(ray.pos) == '/')
        {
          if      (ray.vel.x < 0) ray.vel = Index2D{0, 1};
          else if (ray.vel.x > 0) ray.vel = Index2D{0, -1};
          else if (ray.vel.y < 0) ray.vel = Index2D{1, 0};
          else if (ray.vel.y > 0) ray.vel = Index2D{-1, 0};
        }
        else if (map.Get(ray.pos) == '\\')
        {
          if      (ray.vel.x < 0) ray.vel = Index2D{0, -1};
          else if (ray.vel.x > 0) ray.vel = Index2D{0, 1};
          else if (ray.vel.y < 0) ray.vel = Index2D{-1, 0};
          else if (ray.vel.y > 0) ray.vel = Index2D{1, 0};
        }
        else if (map.Get(ray.pos) == '-')
        {
          bool& hasSplit = splitters[ray.pos];
          if (ray.vel.y != 0 && !hasSplit)
          {
            rays.emplace(Ray{Index2D{ray.pos}, Index2D{-1, 0}});
            rays.emplace(Ray{Index2D{ray.pos}, Index2D{1, 0}});
            hasSplit = true;
            break;
          }
          else if (hasSplit)
          {
            break;
          }
        }
        else if (map.Get(ray.pos) == '|')
        {
          bool& hasSplit = splitters[ray.pos];
          if (ray.vel.x != 0 && !hasSplit)
          {
            rays.emplace(Ray{Index2D{ray.pos}, Index2D{0, -1}});
            rays.emplace(Ray{Index2D{ray.pos}, Index2D{0, 1}});
            hasSplit = true;
            break;
          }
          else if (hasSplit)
          {
            break;
          }
        }
      }
    }
    return energized.Count('#');
  }

  READ_INPUT(input)
  {
    return Input::ReadArray2D(input);
  }

  OUTPUT1(input)
  {
    return Energize(input, Ray{Index2D{-1, 0}, Index2D{1, 0}});
  }

  OUTPUT2(input)
  {
    int max = 0;
    for(int x = 0; x < input.width; x++)
    {
      max = std::max(max, Energize(input, Ray{Index2D{x, -1}, Index2D{0, 1}}));
      max = std::max(max, Energize(input, Ray{Index2D{x, input.height}, Index2D{0, -1}}));
    }
    for(int y = 0; y < input.height; y++)
    {
      max = std::max(max, Energize(input, Ray{Index2D{-1, y}, Index2D{1, 0}}));
      max = std::max(max, Energize(input, Ray{Index2D{input.width, y}, Index2D{-1, 0}}));
    }
    return max;
  }
}
