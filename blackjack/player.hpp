// (c) Matt Garmon - 2017
// player.hpp - Declaration of a player

#ifndef PLAYER
#define PLAYER

#include "card.hpp"

#include <vector>

struct Player {
	std::vector<Card> cards; //hold player hand
	double balance = 0; //hold player balance
	int num_aces = 0; //numer of aces in hand, used for scoring
	int score(); //player's hand score
	bool empty() {return cards.empty();}
	void const print(); //print hand
	void const d_print(); //print dealer hand, only 1st card displayed
};

#endif