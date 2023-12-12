#include "aoc.h"

namespace day12
{
  struct Record
  {
    std::string str;
    std::vector<int> ranges;
  };

  // cache state
  struct State
  {
    int recordIndex;
    int rangeIndex;
    int rangeSize;

    bool operator<(const State& rhs) const
    {
      if (recordIndex != rhs.recordIndex)
        return recordIndex < rhs.recordIndex;
      if (rangeIndex != rhs.rangeIndex)
        return rangeIndex < rhs.rangeIndex;
      return rangeSize < rhs.rangeSize;
    }
  };

  REGISTER_DAY(day12, std::vector<Record>, int64_t);

  REGISTER_TEST_EXAMPLE(day12, ExampleInput, 1, 21);
  REGISTER_TEST        (day12, Input,        1, 7344);
  REGISTER_TEST_EXAMPLE(day12, ExampleInput, 2, 525152);
  REGISTER_TEST        (day12, Input,        2, 1088006519007);

  READ_INPUT(input)
  {
    std::vector<Record> records;
    std::string str;
    while (std::getline(input, str))
    {
      Record record;
      std::stringstream ss{str};
      ss >> record.str;
      int i;
      while (ss >> i)
      {
        record.ranges.emplace_back(i);
        ss >> ",";
      }
      records.emplace_back(record);
    }
    return records;
  }

  int64_t CountRecords(const Record& record, int recordIndex, int rangeIndex, int rangeSize, std::map<State, int64_t>& cache)
  {
    if (recordIndex == record.str.size())
    {
      if (rangeIndex == record.ranges.size() && rangeSize == 0)
        return 1;
      else if (rangeIndex == record.ranges.size() - 1 && rangeSize == record.ranges.back())
        return 1;
      else
        return 0;
    }

    auto it = cache.find(State{recordIndex, rangeIndex, rangeSize});
    if (it != cache.end())
    {
      return it->second;
    }

    int64_t sum = 0;
    if (record.str[recordIndex] == '#' || record.str[recordIndex] == '?')
    {
      sum += CountRecords(record, recordIndex + 1, rangeIndex, rangeSize + 1, cache);
    }
    if(record.str[recordIndex] == '.' || record.str[recordIndex] == '?')
    {
      if(rangeSize == 0)
        sum += CountRecords(record, recordIndex + 1, rangeIndex, rangeSize, cache);
      else if(rangeIndex < record.ranges.size() && rangeSize == record.ranges[rangeIndex])
        sum += CountRecords(record, recordIndex + 1, rangeIndex + 1, 0, cache);
    }

    cache.emplace(State{recordIndex, rangeIndex, rangeSize}, sum);
    return sum;
  }

  int64_t CountRecords(const Record& record)
  {
    std::map<State, int64_t> cache;
    return CountRecords(record, 0, 0, 0, cache);
  }

  OUTPUT1(input)
  {
    int64_t sum = 0;
    for (auto& record : input)
    {
      sum += CountRecords(record);
    }
    return sum;
  }

  OUTPUT2(input)
  {
    int64_t sum = 0;
    for (auto& record : input)
    {
      Record newRecord;
      newRecord.str = Helper::Repeat(record.str + '?', 5);
      newRecord.str.pop_back();
      newRecord.ranges = Helper::Repeat(record.ranges, 5);
      sum += CountRecords(newRecord);
    }
    return sum;
  }
}
