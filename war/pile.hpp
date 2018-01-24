// (c) Matt Garmon - 2017
// pile.hpp - Declaration of a pile

#ifndef PILE
#define PILE

#include "card.hpp"

#include <vector>


struct Pile {
	std::vector<Card> cards;
	bool empty() {return cards.empty();}
	void const print();
	void shuffle();
	void add(Card);
};

#endif