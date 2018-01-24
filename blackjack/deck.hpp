// (c) Matt Garmon - 2017
// deck.hpp - Declaration of Deck

#ifndef DECK
#define DECK

#include "card.hpp"
#include "player.hpp"

#include <vector>

struct Deck {
	std::vector<Card> cards; //holds cards
	void make_standard(int); //make standard deck
	void shuffle(); 
	void const print();

	int first_deal(Player &, Player &);
	int hit(Player &);
};

Deck combine_decks(Deck, Deck);

#endif