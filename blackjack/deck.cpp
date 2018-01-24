// (c) Matt Garmon - 2017
// deck.hpp - Declaration of deck

#include "deck.hpp"

#include <iostream>
#include <random>
#include <algorithm>
#include <cassert>

//makes a standard deck with num_decks decks
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

//shuffles the deck
void Deck::shuffle() {
	if (cards.empty()) return;
	extern std::minstd_rand prng; //sketchy
	std::shuffle(cards.begin(), cards.end(), prng);
}

//prints a deck
void const Deck::print() {
	if (cards.empty()) std::cout << "Empty deck.";
	else {
		for(int i = 1; i <= cards.size(); ++i) {
			std::cout << cards[i-1] << (i%13 == 0 ? '\n' : ' ');
		}
	}
	std::cout << '\n';
}

//combines two decks
Deck combine_decks(Deck d1, Deck d2) {
	Deck d3;
	d3.cards.insert(d3.cards.end(), d1.cards.begin(), d1.cards.end());
	d3.cards.insert(d3.cards.end(), d2.cards.begin(), d2.cards.end());
	return d3;
}

//deals a card to a player
//updates num_aces and returns card counting value
int Deck::hit(Player &p){
	Card c(cards.back().get_rank(), cards.back().get_suit());
	if(c.get_value() == 11) p.num_aces++;
	p.cards.push_back(c);
	cards.pop_back();
	return c.count_value();
}

//deals two cards to the player and dealer
//updates num_aces and returns card counting value
int Deck::first_deal(Player &d, Player &p){
	int count_value = 0;
	for(int i = 0; i < 2; ++i) {
		count_value += hit(p);
		count_value += hit(d);
	}
	return count_value;
}