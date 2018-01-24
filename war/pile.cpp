// (c) Matt Garmon - 2017
// pile.cpp - Implementation of a player

#include "pile.hpp"

#include <iostream>
#include <random>
#include <algorithm>

void const Pile::print() {
	if (cards.empty()) std::cout << "Player has no cards.";
	else {
		for(int i = 1; i <= cards.size(); ++i) {
			std::cout << cards[i-1] << (i%13 == 0 ? '\n' : ' ');
		}
	}
	std::cout << '\n';
}

void Pile::shuffle() {
	if (cards.empty()) return;
	extern std::minstd_rand prng; //sketchy
	std::shuffle(cards.begin(), cards.end(), prng);
}

void Pile::add(Card c){
	cards.insert(cards.begin(), c);
}