#ifndef GAME
#define GAME


#include "deck.hpp"

struct Game {
	Deck d; //holds the cards in play

	int num_decks; //number of decks in play
	double initial_balance; //player inital balance
	bool soft_17; //dealer hit or not hit on soft 17

	int play(); //plays the game
	int cards_remaining() {return d.cards.size();}
	int running_count = 0; //hold the running card count
	double true_count = 0; //hold the true card count
	void update_counts(int); //updates card counts

	//paramaterized constructor for game with settings
	Game(int n_d, double i_b, bool s_17)
		: num_decks(n_d), initial_balance(i_b), soft_17(s_17) {}
};

#endif