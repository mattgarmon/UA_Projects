// (c) Matt Garmon - 2017
// game.cpp - Implementation of Game

#include "game.hpp"
#include "card.hpp"
#include "deck.hpp"
#include "player.hpp"
#include "pile.hpp"

#include <iostream>

Game::Game(int n_d, int n_s, bool s_a, bool ng_s) {
	options.num_decks = n_d; 
	options.num_sacrifices = n_s;
	options.shuffled_aquisition = s_a;
	options.negotiable_sacrifice = ng_s;
}

//game play. 
void Game::play() {
	moves = 0;
	wars = 0;
	
	//initalize a deck
	Deck d;
	d.make_standard(options.num_decks);
	d.shuffle();

	//deal the deck
	Player p1;
	Player p2;
	d.deal(p1, p2);

	//initalize a pile
	Pile pile;

	//play war
	while(true){
		//if anyone is out of cards, end the game and flag p1_win
		if(p1.empty()) {p1_win = false; return;}
		if(p2.empty()) {p1_win = true; return;}

		//get top cards
		Card c1 = p1.take();
		Card c2 = p2.take();
		++moves;

		//add them to the pile
		pile.add(c1);
		pile.add(c2);

		
		if(c1 < c2){ //player 2 wins the round
			p2.give(pile, options.shuffled_aquisition);
			continue;
		}
		else if(c2 < c1){ //player 1 wins the round
			p1.give(pile, options.shuffled_aquisition);
			continue;
		}
		else if(c1 == c2){ //war!
			++wars;

			if(!options.negotiable_sacrifice){
				if(p1.cards.size() < options.num_sacrifices + 1) {
					p1_win = false; return;
				}
				if(p2.cards.size() < options.num_sacrifices + 1) {
					p1_win = true; return;
				}
			}
			
			for(int i = 0; i < options.num_sacrifices; ++i){ //add sacrifices to pile
				//if anyone is out of cards, end the game and flag p1_win
				if(p1.empty()) {p1_win = false; return;}
				if(p2.empty()) {p1_win = true; return;}
				pile.add(p1.take());
				pile.add(p2.take());
			}
			
		}
	}
}