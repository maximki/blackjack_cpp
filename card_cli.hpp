#pragma once

#include "card.hpp"
#include "unordered_map"
#include <string>

namespace CardCLI {
using namespace std::string_literals;
struct FullEn {

    FullEn(const Card& c)
      : card(c) {}

    static const std::unordered_map<Card::Value, std::string> RankStr;
    static const std::unordered_map<Card::Suite, std::string> SuiteStr;

    template<typename Stream>
    friend Stream& operator<<(Stream& os, const FullEn& p) {
        return os << RankStr.at(p.card.value) << " of "
                  << SuiteStr.at(p.card.suite);
    }

  private:
    const Card& card;
};

const std::unordered_map<Card::Value, std::string> FullEn::RankStr{
    { Card::Value::ACE, "Ace"s },   { Card::Value::TWO, "2"s },
    { Card::Value::THREE, "3"s },   { Card::Value::FOUR, "4"s },
    { Card::Value::FIVE, "5"s },    { Card::Value::SIX, "6"s },
    { Card::Value::SEVEN, "7"s },   { Card::Value::EIGHT, "8"s },
    { Card::Value::NINE, "9"s },    { Card::Value::TEN, "10"s },
    { Card::Value::JACK, "Jack"s }, { Card::Value::QUEEN, "Queen"s },
    { Card::Value::KING, "King"s }
};
const std::unordered_map<Card::Suite, std::string> FullEn::SuiteStr{
    { Card::Suite::CLUB, "Club" },
    { Card::Suite::DIAMOND, "Diamond" },
    { Card::Suite::HEART, "Heart" },
    { Card::Suite::SPADE, "Spade" }
};
}