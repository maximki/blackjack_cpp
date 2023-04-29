#pragma once

#include "card.hpp"
#include <vector>

struct Deck {
    Deck() {
        cards.reserve(Card::ALL_VALUES.size() * Card::ALL_SUITES.size());
        for (auto i : Card::ALL_VALUES) {
            for (auto j : Card::ALL_SUITES) {
                cards.emplace_back(i, j);
            }
        }

        shuffle();
    }

    Deck(std::initializer_list<Card> subset) {
        cards.insert(cards.end(), subset.begin(), subset.end());
    }

    void shuffle(size_t times = 200) {
        srand(time(0));

        for (size_t t = 0; t < times; ++t) {
            int i = rand() % 52;
            int j = rand() % 52;

            swap(cards[i], cards[j]);
        }
    }

    Card drawCard() {
        Card card{ cards.back() };
        cards.pop_back();
        return card;
    }

    std::vector<Card> cards;
};
