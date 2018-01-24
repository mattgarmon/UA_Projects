// (c) Matt Garmon - 2017
// deck.hpp - Declaration of Deck

#ifndef DECK
#define DECK

#include "card.hpp"
#include "player.hpp"

struct Deck {
	std::vector<Card> cards;
	void make_standard(int);
	void shuffle();
	void const print();
	void deal(Player &, Player &);
};

Deck combine_decks(Deck, Deck);



#endif