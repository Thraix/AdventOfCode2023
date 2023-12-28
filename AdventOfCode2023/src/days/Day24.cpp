#include "aoc.h"

namespace day24
{
  bool DoubleEq0(double d)
  {
    return std::abs(d) < 0.00000001;
  }

  struct Vec3
  {
    int64_t x = 0;
    int64_t y = 0;
    int64_t z = 0;

    static Vec3 Cross(const Vec3& lhs, const Vec3& rhs)
    {
      return Vec3{lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x};
    }

    Vec3 operator+(const Vec3& rhs) const
    {
      return Vec3{x + rhs.x, y + rhs.y, z + rhs.z};
    }

    Vec3 operator-(const Vec3& rhs) const
    {
      return Vec3{x - rhs.x, y - rhs.y, z - rhs.z};
    }
  };

  struct Vec6
  {
    double data[6];

    Vec6(const Vec3& vec1, const Vec3& vec2)
    {
      data[0] = vec1.x;
      data[1] = vec1.y;
      data[2] = vec1.z;
      data[3] = vec2.x;
      data[4] = vec2.y;
      data[5] = vec2.z;
    }

    Vec6(double i1, double i2, double i3, double i4, double i5, double i6)
    {
      data[0] = i1;
      data[1] = i2;
      data[2] = i3;
      data[3] = i4;
      data[4] = i5;
      data[5] = i6;
    }
  };

  struct Mat6
  {
    double data[6][6];

    Mat6()
    {
      std::memset(data, 0, 6 * 6 * sizeof(double));
    }

    void CrossMatrix(const Vec3& vec, int x, int y)
    {
      data[y][x] = 0;
      data[y][x + 1] = vec.z;
      data[y][x + 2] = -vec.y;
      data[y + 1][x] = -vec.z;
      data[y + 1][x + 1] = 0;
      data[y + 1][x + 2] = vec.x;
      data[y + 2][x] = vec.y;
      data[y + 2][x + 1] = -vec.x;
      data[y + 2][x + 2] = 0;
    }

    void AddRows(Vec6& rhs, int rowTo, int rowFrom, double scale)
    {
      for (int i = 0; i < 6; i++)
      {
        data[rowTo][i] = data[rowTo][i] + data[rowFrom][i] * scale;
      }
      rhs.data[rowTo] = rhs.data[rowTo] + rhs.data[rowFrom] * scale;
    }

    // Find row that can be added to row y to make (x, y) = 0
    // Should follow | 0 0 0 A B | where A is on column x and is not 0
    int FindRow(int x, int y)
    {
      for (int i = 0; i < 6; i++)
      {
        if (i != y && !DoubleEq0(data[i][x]))
        {
          bool found = true;
          for (int j = 0; j < x; j++)
          {
            if (!DoubleEq0(data[i][j]))
            {
              found = false;
              break;
            }
          }
          if (found)
            return i;
        }
      }
      std::cerr << "Didn't find row" << std::endl;
      return 0;
    }

    void NormalizeRow(Vec6& rhs, int y, int x)
    {
      double div = data[y][x];
      for (int i = y; i < 6; i++)
      {
        data[y][i] /= div;
      }
      rhs.data[y] /= div;
    }

    void EliminateDiagonal(Vec6& rhs, int d)
    {
      if (DoubleEq0(data[d][d]))
      {
        for (int i = d + 1; i < 6; i++)
        {
          if (!DoubleEq0(data[i][d]))
          {
            AddRows(rhs, d, i, 1);
            break;
          }
        }
      }
      NormalizeRow(rhs, d, d);
    }

    void EliminateOther(Vec6& rhs, int x, int y)
    {
      if (!DoubleEq0(data[y][x]))
      {
        int row = FindRow(x, y);
        AddRows(rhs, y, row, -data[y][x] / data[row][x]);
      }
    }


    void Eliminate(Vec6& rhs, int x, int y)
    {
      if (x == y)
        EliminateDiagonal(rhs, x);
      else
        EliminateOther(rhs, x, y);
    }

    Vec6 GaussianElemination(Vec6 rhs) const
    {
      Mat6 mat = *this;

      for (int x = 0; x < 6; x++)
      {
        for (int y = 0; y < 6; y++)
        {
          mat.Eliminate(rhs, x, y);
        }
      }
      return rhs;
    }
  };

  struct Wind
  {
    Vec3 pos;
    Vec3 vel;
  };

  REGISTER_DAY(day24, std::vector<Wind>, int64_t);

  REGISTER_TEST_EXAMPLE(day24, ExampleInput, 1, 2);
  REGISTER_TEST        (day24, Input,        1, 18651);
  REGISTER_TEST_EXAMPLE(day24, ExampleInput, 2, 47);
  REGISTER_TEST        (day24, Input,        2, 546494494317645);

  READ_INPUT(input)
  {
    std::string str;
    std::vector<Wind> winds;
    while (std::getline(input, str))
    {
      std::stringstream ss{str};
      Wind wind;
      ss >> wind.pos.x >> "," >> wind.pos.y >> "," >> wind.pos.z >> "@" >> wind.vel.x >> "," >> wind.vel.y >> "," >> wind.vel.z;
      winds.emplace_back(wind);
    }
    return winds;
  }

  bool GetCollisionsXY(const Wind& wind1, const Wind& wind2, int64_t min, int64_t max)
  {
    if (wind1.vel.x * wind2.vel.y == wind2.vel.x * wind1.vel.y)
      return false;

    double t2 = (wind1.vel.y * (wind2.pos.x - wind1.pos.x) - wind1.vel.x * (wind2.pos.y - wind1.pos.y)) / (double)(wind1.vel.x * wind2.vel.y - wind2.vel.x * wind1.vel.y);
    double t1 = ((wind2.pos.x + wind2.vel.x * t2) - wind1.pos.x) / wind1.vel.x;
    if (t1 < 0 || t2 < 0)
      return false;
    double x = wind1.pos.x + wind1.vel.x * t1;
    double y = wind1.pos.y + wind1.vel.y * t1;
    bool isCollision = x >= min && y >= min && x <= max && y <= max;

    return isCollision;
  }

  OUTPUT1(input)
  {
    int64_t min = 7;
    int64_t max = 27;
    if (!isExample)
    {
      min = 200000000000000;
      max = 400000000000000;
    }
    int sum = 0;
    for (int i = 0; i < input.size(); i++)
    {
      for (int j = i + 1; j < input.size(); j++)
      {
        if (GetCollisionsXY(input[i], input[j], min, max))
        {
          sum++;
        }
      }
    }
    return sum;
  }

  OUTPUT2(input)
  {
#if 0
    Derivation:

      p[i] + v[i] * t = p + v * t
      (p - p[i]) = (v - v[i]) * -t // means the vector (p - p[i]) and (v - v[i]) are parallel

      (p - p[i]) x (v - v[i]) == 0
      p x v - p[i] x v - p x v[i] + p[i] x v[i] == 0
      p x v == p[i] x v + p x v[i] - p[i] x v[i]

      p[1] x v + p x v[1] - p[1] x v[1] == p[0] x v + p x v[0] - p[0] x v[0]
      p[2] x v + p x v[2] - p[2] x v[2] == p[0] x v + p x v[0] - p[0] x v[0]

      p x (v[1] - v[0]) + (p[1] - p[0]) x v == p[1] x v[1] - p[0] x v[0]
      p x (v[2] - v[0]) + (p[2] - p[0]) x v == p[2] x v[2] - p[0] x v[0]

      vij = v[i] - v[j]
      pij = p[i] - p[j]
      Cij = p[j] x v[j] - p[i] x v[i]

      p x v10 - p10 x v == C10;
      p x v20 - p20 x v == C10;

      |     0   v10z  -v10y      0   p10z  -p10y | C10x |
      | -v10z      0   v10x  -p10z      0   p10x | C10y |
      |  v10y  -v10x      0   p10y  -p10x      0 | C10z |
      |     0   v20z  -v20y      0   p20z  -p20y | C20x |
      | -v20z      0   v20x  -p20z      0   p20x | C20y |
      |  v20y  -v20x      0   p20y  -p20x      0 | C20z |
#endif

    Mat6 mat;
    mat.CrossMatrix(input[1].vel - input[0].vel, 0, 0);
    mat.CrossMatrix(input[2].vel - input[0].vel, 0, 3);
    mat.CrossMatrix(input[1].pos - input[0].pos, 3, 0);
    mat.CrossMatrix(input[2].pos - input[0].pos, 3, 3);
    Vec3 C10 = Vec3::Cross(input[1].pos, input[1].vel) - Vec3::Cross(input[0].pos, input[0].vel);
    Vec3 C20 = Vec3::Cross(input[2].pos, input[2].vel) - Vec3::Cross(input[0].pos, input[0].vel);
    Vec6 rhs{C10, C20};
    Vec6 res = mat.GaussianElemination(rhs);

    return int64_t(res.data[0]) + int64_t(res.data[1]) + int64_t(res.data[2]);
  }
}
