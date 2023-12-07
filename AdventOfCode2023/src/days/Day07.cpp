#include "aoc.h"

namespace day07
{
  int CalcStrength(const std::string& hand)
  {
    std::map<char, int> score;
    for (char c : hand)
    {
      score[c]++;
    }
    if (score.size() == 1)
      return 7; // Five of a kind
    else if(score.size() == 2)
    {
      if (score.begin()->second == 4 || score.begin()->second == 1)
        return 6; // Four of a kind
      else
        return 5; // Full house
    }
    else if (score.size() == 3)
    {
      if (score.begin()->second == 2 || (++score.begin())->second == 2)
        return 3; // Two pairs
      else
        return 4; // Three of a kind
    }
    else if(score.size() == 4)
      return 2; // One pair
    else
      return 1; // High card
  }
  struct Hand
  {
    std::string hand;
    int bet;

    int strength = -1;

    void JokerStrength()
    {
      std::string replaces = "AKQT987654321";
      int maxStrength = 0;
      char maxC;
      for (auto c : replaces)
      {
        std::string newHand = hand;
        std::replace(newHand.begin(), newHand.end(), 'J', c);
        int strength = CalcStrength(newHand);
        if(strength > maxStrength)
        {
          maxStrength = strength;
          maxC = c;
        }
      }

      std::string newHand = hand;
      std::replace(newHand.begin(), newHand.end(), 'J', maxC);

      strength = CalcStrength(newHand);
      std::replace(hand.begin(), hand.end(), 'J', '0');
    }

    int GetValue(char c) const
    {
      if (Helper::IsDigit(c))
        return c - '0';
      else if (c == 'T')
        return 10;
      else if (c == 'J')
        return 11;
      else if (c == 'Q')
        return 12;
      else if (c == 'K')
        return 13;
      else if (c == 'A')
        return 14;
      std::cout << "Invalid card value" << std::endl;
      return 0;
    }

    bool operator<(const Hand& rhs) const
    {
      if (strength != rhs.strength)
        return strength < rhs.strength;

      for (int i = 0; i < hand.size(); i++) {
        if (hand[i] != rhs.hand[i])
        {
          return GetValue(hand[i]) < GetValue(rhs.hand[i]);
        }
      }
      std::cout << "Same strength is invalid" << std::endl;
      return false;
    }

  };

  REGISTER_DAY(day07, std::vector<Hand>, int);

  REGISTER_TEST_EXAMPLE(day07, ExampleInput, 1, 6440);
  REGISTER_TEST        (day07, Input,        1, 249390788);
  REGISTER_TEST_EXAMPLE(day07, ExampleInput, 2, 5905);
  REGISTER_TEST        (day07, Input,        2, 248750248);

  READ_INPUT(input)
  {
    std::string str;
    std::vector<Hand> hands;
    while (std::getline(input, str))
    {
      std::stringstream ss{str};
      Hand hand;
      ss >> hand.hand >> hand.bet;
      hands.emplace_back(hand);
    }
    return hands;
  }

  OUTPUT1(input)
  {
    std::vector<Hand> cpy = input;
    for(int i = 0; i < cpy.size(); i++)
    {
      cpy[i].strength = CalcStrength(cpy[i].hand);
    }
    std::sort(cpy.begin(), cpy.end());

    int sum = 0;
    for (int i = 0; i < cpy.size(); i++)
    {
      sum += cpy[i].bet * (i + 1);
    }

    return sum;
  }

  OUTPUT2(input)
  {
    std::vector<Hand> cpy = input;
    for(int i = 0; i < cpy.size(); i++)
    {
      cpy[i].JokerStrength();
    }
    std::sort(cpy.begin(), cpy.end());

    int sum = 0;
    for (int i = 0; i < cpy.size(); i++)
    {
      sum += cpy[i].bet * (i + 1);
    }
    return sum;
  }
}
