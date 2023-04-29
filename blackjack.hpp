#pragma once

#include "card.hpp"
#include "card_cli.hpp"
#include "deck.hpp"

#include <functional>
#include <iomanip>
#include <map>
#include <unordered_map>

namespace BlackJack {

struct Person {
    std::vector<Card> cardsOnHand;

    static constexpr int MaxScore = 21;
    static const std::unordered_map<Card::Value, int> MinScores;

    int sum() const {
        // There can be multiple aces but only one of them can be valued at 11
        // points because 11+11=22 and it puts a player/dealer over the limit
        bool hasAnAce = false;
        int score     = 0;
        for (const auto& card : cardsOnHand) {
            hasAnAce |= (card.value == Card::Value::ACE);
            score += MinScores.at(card.value);
        }

        if (hasAnAce && score > MaxScore) { score -= 10; }
        return score;
    };

    template<typename Stream>
    friend Stream& operator<<(Stream& os, const Person& p) {
        for (const auto& card : p.cardsOnHand) {
            os << CardCLI::FullEn(card) << "\n";
        }
        os << "Score: " << p.sum();
        if (p.hasBlackjack()) os << ", BlackJack!";
        else if (p.busted()) os << ", busted!";

        return os << "\n";
    }

    bool busted() const { return sum() > MaxScore; }
    bool hasBlackjack() const { return sum() == MaxScore; }
    void reset() { cardsOnHand.clear(); }

    void getCard() {}
};

const std::unordered_map<Card::Value, int> Person::MinScores{
    { Card::Value::ACE, 11 },  { Card::Value::TWO, 2 },
    { Card::Value::THREE, 3 }, { Card::Value::FOUR, 4 },
    { Card::Value::FIVE, 5 },  { Card::Value::SIX, 6 },
    { Card::Value::SEVEN, 7 }, { Card::Value::EIGHT, 8 },
    { Card::Value::NINE, 9 },  { Card::Value::TEN, 10 },
    { Card::Value::JACK, 10 }, { Card::Value::QUEEN, 10 },
    { Card::Value::KING, 10 }
};

struct Dealer : Person {
    static constexpr int DealerMaxScoreToHit = 17;

    Dealer() = default;
    // An ability to insert a special deck is there for testing
    Dealer(Deck&& cards)
      : deck(std::move(cards)) {}

    Card draw() { return deck.drawCard(); }
    void getCard() { cardsOnHand.push_back(draw()); }
    bool canDraw() { return sum() < DealerMaxScoreToHit; }

    void reset() {
        deck = Deck{};
        Person::reset();
    }
    void reset(Deck&& cards) {
        deck = std::move(cards);
        Person::reset();
    }

    template<typename Stream>
    friend Stream& operator<<(Stream& os, const Dealer& d) {
        return os << "Dealer's hand\n" << static_cast<Person>(d);
    }

  private:
    Deck deck;
};

struct Player : Person {
    bool standing = false;

    Player(Dealer& d)
      : dealer(d) {}

    Dealer& dealer;

    void getCard() { cardsOnHand.push_back(dealer.draw()); }

    template<typename Stream>
    friend Stream& operator<<(Stream& os, const Player& p) {
        return os << "Player's hand\n" << static_cast<Person>(p);
    }

    void reset() {
        standing = false;
        Person::reset();
    }
};

class Game {
  public:
    Game()
      : player(dealer) {}

    Game(Dealer&& d)
      : dealer(std::move(d))
      , player(dealer) {}

    void play() {
        printRules();

        while (!exit) {
            playARound();
            reset();
        }
        printGameSummary();
    }

    void printRules() {
        std::cout
          << "It's a simplyfied blackjack.\n\n"
          << "There is 1 player (you) and a dealer (program). The player\n"
             "starts with 2 cards while the dealer starts with 1. The player\n"
             "can see that dealer's card. The dealer doesn't know player's\n"
             "score if it's not over 21. Card values are the same as in \n"
             "regular blackjack with aces valued at 11 or 1 (but only 1 ace\n"
             "on hand can be valued as 1). The player can only hit or stand,\n"
             "there are no chips, no splitting, no surrender. The player\n"
             "draws cards first until they decide to stand or they bust. The\n"
             "dealer draws as many cards as it can until it hits \"soft 17\"\n"
             "(for example, {a 10 and an ace}, or {a 5, an ace, and a 3}, or\n"
             "{2 aces and a 5}). If the player busts then the dealer wins\n"
             "automatically. If the dealer busts then the player wins. If\n"
             "there is a tie (or both have blackjacks), no one wins.\n"
             "Otherwise, whoever has the highest score wins.\n"
          << "--------------------------------------------------\n"
          << std::endl;
    }

  private:
    void reset() {
        dealer.reset();
        player.reset();
    }

    void init() {
        dealer.getCard();
        player.getCard();
        player.getCard();
        exit = false;
    }

    bool isPlayerDoneHitting() const {
        return exit || player.busted() || player.hasBlackjack() ||
               player.standing;
    }

    void playARound() {
        init();
        ++roundsPlayed;
        printStartRoundMessage();
        std::cout << dealer << "\n";
        std::cout << player << "\n";
        while (!isPlayerDoneHitting()) {
            processUserInput();
        }
        if (exit) return;
        if (!player.busted()) {
            while (dealer.canDraw()) {
                dealer.getCard();
            }
            std::cout << dealer << std::endl;
        }
        processRoundResults();
    }

    void processRoundResults() {
        auto playerWon = [&] {
            std::cout << "Player won!\n" << std::endl;
            ++roundsWonByUser;
        };
        auto dealerWon = [&] {
            std::cout << "Dealer won this time.\n" << std::endl;
        };
        auto itIsATie = [&] { std::cout << "It is a tie.\n" << std::endl; };

        if (player.busted()) dealerWon();
        else if (dealer.busted()) playerWon();
        else if (player.sum() == dealer.sum()) itIsATie();
        else if (player.hasBlackjack() || player.sum() > dealer.sum())
            playerWon();
        else dealerWon();
    }

    void printStartRoundMessage() const {
        std::cout << "=== Round #" << roundsPlayed << " ===\n";
    }

    void processUserInput() {
        promptUser("Enter your choice ([H]it, [S]tand, [Q]uit): ",
                   { { 'H',
                       [&] {
                           player.getCard();
                           std::cout << player << std::endl;
                       } },
                     { 'S', [&] { player.standing = true; } },
                     { 'Q', [&] { exit = true; } } });
    }

    void promptUser(std::string_view prompt,
                    std::map<char, std::function<void()>> choices) const {
        std::cout << prompt;
        char response;

        auto iter = choices.end();
        do {
            std::cin >> std::setw(1) >> response;
            response = toupper(response);
            iter     = choices.find(response);
        } while (iter == choices.end());

        std::cout << "\n";
        iter->second();
    }

    void printGameSummary() const {
        std::cout << "Rounds played: " << roundsPlayed << "\n";
        std::cout << "Player won   : " << roundsWonByUser << "\n";
    }

    Dealer dealer;
    Player player;

    bool exit = false;

    size_t roundsPlayed    = 0;
    size_t roundsWonByUser = 0;
};

} // namespace BlackJack