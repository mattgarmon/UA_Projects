// (c) Matt Garmon - 2017
// player.cpp - Implementation of a player

#include "player.hpp"
#include <iostream>

//returns player hand score
int Player::score() {
	int s = 0;

	//get player hand score
	for(int i = 0; i < cards.size(); ++i) {
		s += cards[i].get_value();
	}
	if(s > 21){ //handle multiple aces case
		int aces = num_aces;
		if (aces > 0){
			while(s > 21 && aces > 0){
				s -= 10;
				--aces;
			}
		}
	}
	return s;
}

//print a player hand
void const Player::print() {
	if (cards.empty()) std::cout << "Player has no cards.";
	else {
		for(int i = 0; i < cards.size(); ++i) {
			std::cout << cards[i] << ' ';
		}
	}
	std::cout << '\n';
}

//print intial dealer hand
void const Player::d_print() {
	if (cards.empty()) std::cout << "Player has no cards.";
	else {
		std::cout << cards[0] << ' ' << "[?]";
	}
	std::cout << '\n';
}