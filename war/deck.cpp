// (c) Matt Garmon - 2017
// deck.hpp - Declaration of deck

#include "deck.hpp"

#include <iostream>
#include <random>
#include <algorithm>
#include <cassert>

void Deck::make_standard(int num_decks) {
	if(cards.size() != 0) cards.clear();
	for(int d = 0; d < num_decks; ++d){
		for(int s = Clubs; s <= Spades; ++s) {
			for(int r = Ace; r <= King; ++r) {
				Card c(static_cast<Rank>(r), static_cast<Suit>(s));
				cards.push_back(c);
			}
		}
	}
}

void Deck::shuffle() {
	if (cards.empty()) return;
	extern std::minstd_rand prng; //sketchy
	std::shuffle(cards.begin(), cards.end(), prng);
}

void const Deck::print() {
	if (cards.empty()) std::cout << "Empty deck.";
	else {
		for(int i = 1; i <= cards.size(); ++i) {
			std::cout << cards[i-1] << (i%13 == 0 ? '\n' : ' ');
		}
	}
	std::cout << '\n';
}

void Deck::deal(Player& p1, Player& p2){
	size_t s = cards.size();
	assert(cards.size()%2 == 0);
	for(int i = 0; i < s/2; ++i){
		p1.cards.push_back(cards.back());
		cards.pop_back();
		p2.cards.push_back(cards.back());
		cards.pop_back();
	}
}

Deck combine_decks(Deck d1, Deck d2) {
	Deck d3;
	d3.cards.insert(d3.cards.end(), d1.cards.begin(), d1.cards.end());
	d3.cards.insert(d3.cards.end(), d2.cards.begin(), d2.cards.end());
	return d3;
}

