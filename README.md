# Overview
It's a simplyfied blackjack.

There is 1 player (you) and a dealer (program). The player
starts with 2 cards while the dealer starts with 1. The player
can see that dealer's card. The dealer doesn't know player's
score if it's not over 21. Card values are the same as in 
regular blackjack with aces valued at 11 or 1 (but only 1 ace
on hand can be valued as 1). The player can only hit or stand,
there are no chips, no splitting, no surrender. The player draws
cards first until they decide to stand or they bust. The dealer
draws as many cards as it can until it hits "soft 17"
(for example, {a 10 and an ace}, or {a 5, an ace, and a 3}, or
{2 aces and a 5}). If the player busts then the dealer wins
automatically. If the dealer busts then the player wins. If
there is a tie (or both have blackjacks), no one wins.
Otherwise, whoever has the highest score wins.

# Compilation
For simplicity all classes are defined in header files. It's not the best practice, but everything compiles with 1 command:
```
g++ -std=c++20 -g main.cpp -o blackjack
```

# Example
```
=== Round #1 ===
Dealer's hand
7 of Spade
Score: 7

Player's hand
2 of Diamond
10 of Diamond
Score: 12

Enter your choice ([H]it, [S]tand, [Q]uit): H

Player's hand
2 of Diamond
10 of Diamond
10 of Club
Score: 22, busted!

Dealer won this time.

=== Round #2 ===
Dealer's hand
6 of Diamond
Score: 6

Player's hand
6 of Heart
2 of Heart
Score: 8

Enter your choice ([H]it, [S]tand, [Q]uit): H

Player's hand
6 of Heart
2 of Heart
7 of Club
Score: 15

Enter your choice ([H]it, [S]tand, [Q]uit): H

Player's hand
6 of Heart
2 of Heart
7 of Club
Ace of Diamond
Score: 16

Enter your choice ([H]it, [S]tand, [Q]uit): H

Player's hand
6 of Heart
2 of Heart
7 of Club
Ace of Diamond
4 of Heart
Score: 20

Enter your choice ([H]it, [S]tand, [Q]uit): S

Dealer's hand
6 of Diamond
9 of Club
10 of Diamond
Score: 25, busted!

Player won!

=== Round #3 ===
Dealer's hand
4 of Diamond
Score: 4

Player's hand
6 of Spade
King of Club
Score: 16

Enter your choice ([H]it, [S]tand, [Q]uit): q

Rounds played: 3
Player won   : 1
```
