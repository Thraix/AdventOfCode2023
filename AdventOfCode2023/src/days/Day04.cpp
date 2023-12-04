#include "aoc.h"

namespace day04
{

  struct Game
  {
    std::vector<int> winning;
    std::set<int> numbers;
  };

  REGISTER_DAY(day04, std::vector<Game>, int);

  REGISTER_TEST_EXAMPLE(day04, ExampleInput, 1, 13);
  REGISTER_TEST        (day04, Input,        1, 23678);
  REGISTER_TEST_EXAMPLE(day04, ExampleInput, 2, 30);
  REGISTER_TEST        (day04, Input,        2, 15455663);

  READ_INPUT(input)
  {
    std::string str;
    std::vector<Game> games;
    while (std::getline(input, str))
    {
      Game game;
      int i;
      int pos = str.find("|");

      std::stringstream ss{str};
      ss >> "Card " >> i >> ":";
      while (ss >> i)
        game.winning.emplace_back(i);
      ss.clear(); // clear the failbit to be able to read more data

      ss >> "|";

      while (ss >> i)
        game.numbers.emplace(i);

      games.emplace_back(game);
    }
    return games;
  }

  OUTPUT1(input)
  {
    int sum = 0;
    for (auto& game : input)
    {
      int score = 0;
      for (int winning : game.winning)
      {
        if (game.numbers.count(winning))
        {
          if (score == 0) score = 1;
          else score *= 2;
        }
      }
      sum += score;
    }
    return sum;
  }

  OUTPUT2(input)
  {
    std::vector<int> scratches(input.size(), 1);
    int sum = 0;
    for (int game = 0; game < input.size(); game++)
    {
      int matches = 0;
      for (int winning : input[game].winning)
      {
        if (input[game].numbers.count(winning))
        {
          matches++;
        }
      }
      for (int i = 0; i < matches; i++)
      {
        scratches[game + i + 1] += scratches[game];
      }
    }
    for (int i = 0; i < scratches.size(); i++)
    {
      sum += scratches[i];
    }
    return sum;
  }
}
