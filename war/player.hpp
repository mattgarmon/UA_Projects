// (c) Matt Garmon - 2017
// player.hpp - Declaration of a player

#ifndef PLAYER
#define PLAYER

#include "card.hpp"
#include "pile.hpp"
#include <vector>

struct Player {
	std::vector<Card> cards;
	bool empty() {return cards.empty();}
	void const print();
	Card take();
	void give(Pile&, bool);
	
};

#endif