#include "aoc.h"

namespace day17
{
  struct State
  {
    Index2D position;
    Index2D direction;
    int amount;

    bool operator<(const State& rhs) const
    {
      if (position != rhs.position)
        return position < rhs.position;
      if (direction != rhs.direction)
        return direction < rhs.direction;
      return amount < rhs.amount;
    }

    friend std::ostream& operator<<(std::ostream& stream, const State& state)
    {
      return stream << state.position << " " << state.direction << " " << state.amount;
    }
  };

  std::vector<std::pair<int, State>> Branch(const Array2D<int>& map, const State& state)
  {
    std::vector<std::pair<int, State>> states;
    if (state.amount != 3)
    {
      Index2D newPos = state.position + state.direction;
      if(map.IsInside(newPos))
        states.emplace_back(map.Get(newPos), State{newPos, state.direction, state.amount + 1});
    }

    Index2D newPos1 = state.position + Index2D{state.direction.y, state.direction.x};
    if(map.IsInside(newPos1))
      states.emplace_back(map.Get(newPos1), State{newPos1, Index2D{state.direction.y, state.direction.x}, 1});

    Index2D newPos2 = state.position + Index2D{-state.direction.y, -state.direction.x};
    if(map.IsInside(newPos2))
      states.emplace_back(map.Get(newPos2), State{newPos2, Index2D{-state.direction.y, -state.direction.x}, 1});
    return states;
  }

  std::vector<std::pair<int, State>> Branch2(const Array2D<int>& map, const State& state)
  {
    std::vector<std::pair<int, State>> states;
    if (state.amount < 10)
    {
      Index2D newPos = state.position + state.direction;
      if(map.IsInside(newPos))
        states.emplace_back(map.Get(newPos), State{newPos, state.direction, state.amount + 1});
    }
    if (state.amount >= 4)
    {
      Index2D newPos1 = state.position + Index2D{state.direction.y, state.direction.x};
      if (map.IsInside(newPos1))
        states.emplace_back(map.Get(newPos1), State{newPos1, Index2D{state.direction.y, state.direction.x}, 1});

      Index2D newPos2 = state.position + Index2D{-state.direction.y, -state.direction.x};
      if (map.IsInside(newPos2))
        states.emplace_back(map.Get(newPos2), State{newPos2, Index2D{-state.direction.y, -state.direction.x}, 1});
    }
    return states;
  }

  bool Goal(const Array2D<int>& map, const State& state)
  {
    return state.position.x == map.width - 1 && state.position.y == map.height - 1;
  }

  bool Goal2(const Array2D<int>& map, const State& state)
  {
    return state.amount >= 4 && state.position.x == map.width - 1 && state.position.y == map.height - 1;
  }

  REGISTER_DAY(day17, Array2D<int>, int);

  REGISTER_TEST_EXAMPLE(day17, ExampleInput, 1, 102);
  REGISTER_TEST        (day17, Input,        1, 845);
  REGISTER_TEST_EXAMPLE(day17, ExampleInput, 2, 94);
  REGISTER_TEST        (day17, Input,        2, 993);

  READ_INPUT(input)
  {
    return Input::ReadDigitsAsArray2D(input);
  }

  OUTPUT1(input)
  {
    State startRight{Index2D{0, 0}, Index2D{1, 0}, 0};
    State startDown{Index2D{0, 0}, Index2D{0, 1}, 0};
    return Helper::Dijkstras(input, std::vector<State>{startRight, startDown}, Branch, Goal);
  }

  OUTPUT2(input)
  {
    State startRight{Index2D{0, 0}, Index2D{1, 0}, 0};
    State startDown{Index2D{0, 0}, Index2D{0, 1}, 0};
    return Helper::Dijkstras(input, std::vector<State>{startRight, startDown}, Branch2, Goal2);
  }
}
