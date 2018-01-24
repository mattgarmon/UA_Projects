#include "game.hpp"

#include <iostream>
#include <random>
#include <algorithm>

std::minstd_rand prng; //pseudo random number generator
std::random_device rng; //random device

int main() {
	prng.seed(rng());
	
	char input; //for new game input

	Game g(4, 100, true); //4 decks, $100 to start, dealer hits on soft 17
	
	while(true){
		g.play(); //play the game
		
		//see if the player wants to play again
		std::cout << "Would you like to play again (y/n)? ";
		std::cin >> input;
		std::cin.ignore();
		if(input == 'y') continue; //play new game
		else break; //end the program
	}

	return 0;
}