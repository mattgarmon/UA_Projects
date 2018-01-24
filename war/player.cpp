// (c) Matt Garmon - 2017
// player.cpp - Implementation of a player

#include "player.hpp"
#include <iostream>

void const Player::print() {
	if (cards.empty()) std::cout << "Player has no cards.";
	else {
		for(int i = 1; i <= cards.size(); ++i) {
			std::cout << cards[i-1] << (i%13 == 0 ? '\n' : ' ');
		}
	}
	std::cout << '\n';
}

Card Player::take() {
	Card c = cards.back();
	cards.pop_back();
	return c;
}

void Player::give(Pile &p, bool random_aquisition) {
	if(random_aquisition) p.shuffle(); //random aquisiton
	for(Card c : p.cards){
		cards.insert(cards.begin(), c);
	}
	p.cards.clear();
}