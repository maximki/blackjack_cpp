#pragma once

#include <array>
#include <iostream>
#include <limits>
#include <set>
#include <vector>

struct Card {

    enum class Suite : uint8_t { CLUB, DIAMOND, HEART, SPADE };
    static constexpr std::array<Suite, 4> ALL_SUITES{ Suite::CLUB,
                                                      Suite::DIAMOND,
                                                      Suite::HEART,
                                                      Suite::SPADE };

    enum class Value : uint8_t {
        ACE = 1,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
        NINE,
        TEN,
        JACK,
        QUEEN,
        KING
    };
    static constexpr std::array<Value, 13> ALL_VALUES{
        Value::ACE,  Value::TWO,   Value::THREE, Value::FOUR, Value::FIVE,
        Value::SIX,  Value::SEVEN, Value::EIGHT, Value::NINE, Value::TEN,
        Value::JACK, Value::QUEEN, Value::KING
    };

    Card() = delete;
    Card(Value v, Suite s)
      : value(v)
      , suite(s){};
    Card(const Card& card)                 = default;
    Card(Card&& card)                      = default;
    constexpr Card& operator=(const Card&) = default;

    Value value;
    Suite suite;
};

void
swap(Card& lhs, Card& rhs) {
    std::swap(lhs.value, rhs.value);
    std::swap(lhs.suite, rhs.suite);
}
