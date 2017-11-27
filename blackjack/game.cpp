#include "game.hpp"
#include "player.hpp"
#include "deck.hpp"

#include <iostream>

//updates card counts
void Game::update_counts(int count_value){
	running_count += count_value;
	true_count = (double)running_count/((double)cards_remaining()/(double)52);
}

//plays the game
int Game::play() {
	d.make_standard(num_decks); //get a deck
	d.shuffle();

	Player dealer;
	Player player;
	player.balance = initial_balance; //inital player balance

	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	std::cout << "           \u2663 \u2666 \u2665 \u2660 Welcome to Blackjack! \u2663 \u2666 \u2665 \u2660\n";
	std::cout << "You will start with a balance of $" << initial_balance << ". Press enter to start.";
	std::cin.ignore();
	
	int input = 0; //for player input
	double bet = 0; //for player bet
	
	//make sure card counts are zero
	running_count = 0;
	true_count = 0;

	start: //start label used to return to anew round

	while(player.balance > 0){ //while the player has a balance, loop the game
		
		//reset hands
		dealer.cards.clear();
		dealer.num_aces = 0;
		player.cards.clear();
		player.num_aces = 0;

		std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

		//reshuffle the deck at 25%
		if(d.cards.size() <= (num_decks*52)/4) {
			std::cout << "Deck at 25%. Reshuffling...\n\n";
			d.cards.clear();
			d.make_standard(num_decks);
			d.shuffle();
			running_count = 0;
			true_count = 0;
		}

		//display updated card counts
		std::cout << "Running count: " << running_count;
		std::cout << "\nTrue count: " << true_count << '\n';


		//get player bet
		std::cout << "Balance: $" << player.balance;
		std::cout << "\nWhat is your bet? $";
		std::cin >> bet;
		std::cin.ignore();
		while(bet > player.balance || bet < 0) {
			std::cout << "Please enter valid input. What is your bet? ";
			std::cin >> bet;
			std::cin.ignore();
		}
		player.balance -= bet;

		//deal the cards
		update_counts(d.first_deal(dealer, player));
		std::cout << "\nDealer is showing: "; 
		dealer.d_print();
		std::cout << "\nYou have: ";
		player.print();

		//hold the players score
		int score = player.score();

		//if blackjack right away, pay 3:2
		if(score == 21) {
			std::cout << "\nBlackjack! You won this round.\n";
			player.balance += bet*2.5;
			goto start;
		}

		while(score < 22){  //while the player hasn't busted

			//get user input
			std::cout << "Your current score is: " << player.score();
			std::cout << "\nWould you like to hit(1) or stand(2)? ";
			std::cin >> input;
			std::cin.ignore();

			//play the player's hand
			if(input == 2) break; //player is standing
			else if(input == 1){ //payer is hitting
				update_counts(d.hit(player));
				score = player.score();
				if(score > 21){ //player busted
					std::cout << "\nYou have: ";
					player.print();
					std::cout << "Your score: " << player.score();
					std::cout << "\n\nBust! Dealer wins this round.\n";
					goto start;
				}
				std::cout << "\nYou have: "; //show updated hand
				player.print();
			}
			else {
				std::cout << "\nPlease enter valid input.\n";
				continue; //go back to input
			}
		}

		//play the dealer's hand with soft 17 option
		int d_score = dealer.score();
		std::cout << std::endl;
		std::cout << "Dealer has: "; 
		dealer.print();
		while(d_score < 17 || (soft_17 && d_score == 17 && dealer.num_aces > 0)){ 
			update_counts(d.hit(dealer));
			d_score = dealer.score();
			std::cout << "Dealer drew: "; 
			dealer.print();
			if(d_score > 21){ //the dealer busted
				std::cout << "Dealer score: " << dealer.score();
				std::cout << "\n\nDealer Bust! You win this round.\n";
				player.balance += bet*2;
				goto start;
			}
		}
		
		//determine results of round
		std::cout << "Dealer score: " << dealer.score();
		if (score == d_score) {
			std::cout << "\n\nPush.\n";
			player.balance += bet;
			goto start;
		}
		else if(score > d_score){
			std::cout << "\n\nYou win this round!\n";
			player.balance += bet*2;
			goto start;
		}
		else{
			std::cout << "\n\nDealer wins this round.\n";
			goto start;
		}
	}

	//player is out of money, end the game
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	std::cout << "Looks like you're out of money...\n";
	return 0;
}