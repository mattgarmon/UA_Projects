// (c) Matt Garmon - 2017
// main.cpp - War simulation

#include "game.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>

std::minstd_rand prng; //pseudo random number generator
std::random_device rng; //random device

int main(int argc, char* argv[]) {
	prng.seed(rng());

	bool default_options = false;
	if(argc == 2 || argc == 1) default_options = true; 

	int num_tests = 10000;
	if(argc == 2 || !default_options) num_tests = atoi(argv[1]);

	int moves_total = 0;
	int wars_total = 0;
	int player_1_wins = 0;

	for(int i = 0; i < num_tests; ++i){
		Game g;
		if(!default_options) {
			g.options.num_decks = atoi(argv[2]);
			g.options.num_sacrifices = atoi(argv[3]);
			g.options.shuffled_aquisition = (std::string(argv[4]) == "y" ? true : false);
			g.options.negotiable_sacrifice = (std::string(argv[5]) == "y" ? true : false);
		}
		g.play();
		moves_total += g.moves;
		wars_total += g.wars;
		if(g.p1_win) ++player_1_wins;
	}
	
	std::cout << "P1 wins: " << player_1_wins << std::endl;
	std::cout << "P2 wins: " << (num_tests - player_1_wins)  << std::endl;
	std::cout << "Average moves: " << (double)moves_total/(double)num_tests << std::endl;
	std::cout << "Average wars: " << (double)wars_total/(double)num_tests << std::endl;

	return 0;
}