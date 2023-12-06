#include "aoc.h"

namespace day05
{
  struct Range
  {
    int64_t start;
    int64_t length;
    friend std::ostream& operator<<(std::ostream& stream, const Range& range)
    {
      return stream << range.start << " " << range.length;
    }
  };
  struct Plantation
  {
    std::vector<int64_t> seeds;
    std::vector<std::map<int64_t, Range>> maps;
  };

  REGISTER_DAY(day05, Plantation, int64_t);

  REGISTER_TEST_EXAMPLE(day05, ExampleInput, 1, 35);
  REGISTER_TEST        (day05, Input,        1, 88151870);
  REGISTER_TEST_EXAMPLE(day05, ExampleInput, 2, 46);
  REGISTER_TEST        (day05, Input,        2, 2008785);

  int64_t FindMap(const std::map<int64_t, Range>& map, int64_t value)
  {
    for (auto& range : map)
    {
      if (value >= range.first && value < range.first + range.second.length)
      {
        return range.second.start + (value - range.first);
      }
    }
    return value;
  }

  int64_t FindLocation(const Plantation& plantation, int64_t seed)
  {
    int64_t value = seed;
    for (auto& ranges : plantation.maps)
    {
      value = FindMap(ranges, value);
    }
    return value;
  }

  Range FindRange(const std::map<int64_t, Range>& map, Range valueRange)
  {
    for (auto& range : map)
    {
      if (valueRange.start >= range.first && valueRange.start < range.first + range.second.length)
      {
        return Range{range.second.start + valueRange.start - range.first, range.second.length - (valueRange.start - range.first)};
      }
    }
    int64_t length = 0;
    for (auto& range : map)
    {
      if (valueRange.start < range.first)
      {
        length = range.first - valueRange.start;
        break;
      }
    }
    if (length == 0)
      length = valueRange.length;

    return Range{valueRange.start, length};
  }

  READ_INPUT(input)
  {
    std::string str;
    Plantation plantation;

    std::getline(input, str);
    std::stringstream ss{str};
    int64_t i;
    ss >> "seeds: ";
    while (ss >> i)
      plantation.seeds.emplace_back(i);
    std::getline(input, str);
    while(std::getline(input, str))
    {
      std::map<int64_t, Range> ranges;
      std::getline(input, str);
      while (!str.empty())
      {
        int64_t source, destination, range;
        std::stringstream ss{str};
        ss >> destination >> source >> range;
        ranges.emplace(source, Range{destination, range});
        str = "";
        std::getline(input, str);
      }
      plantation.maps.emplace_back(ranges);
    }

    return plantation;
  }

  OUTPUT1(input)
  {
    int64_t min = FindLocation(input, input.seeds.front());
    for (auto seed : input.seeds)
    {
      min = std::min(min, FindLocation(input, seed));
    }
    return min;
  }

  void GetRanges(Range range, const std::map<int64_t, Range>& toRanges, std::vector<Range>& resRanges)
  {
    while (true)
    {
      Range newRange = FindRange(toRanges, range);
      if (range.length <= newRange.length)
      {
        newRange.length = range.length;
        resRanges.emplace_back(newRange);
        break;
      }
      resRanges.emplace_back(newRange);
      range.start += newRange.length;
      range.length -= newRange.length;
    }
  }

  OUTPUT2(input)
  {
    std::vector<Range> ranges;
    for (int i = 0; i < input.seeds.size(); i+=2)
    {
      ranges.emplace_back(Range{input.seeds[i], input.seeds[i + 1]});
    }
    for (auto map : input.maps)
    {
      std::vector<Range> newRanges;
      for (auto& range : ranges)
      {
        GetRanges(range, map, newRanges);
      }
      ranges = newRanges;
    }

    int64_t min = ranges.front().start;
    for (auto range : ranges)
    {
      min = std::min(min, range.start);
    }
    return min;
  }
}
