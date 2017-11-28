// (c) Matt Garmon - 2017
// game.hpp - Declaration of Game

#ifndef GAME
#define GAME


struct Game {
	int moves = 0;
	int wars = 0;
	bool p1_win = false;

	struct Options{
		int num_decks = 1;
		int num_sacrifices = 3;
		bool shuffled_aquisition = true;
		bool negotiable_sacrifice = true;
	};
	Options options;

	Game() = default;
	Game(int, int, bool, bool);

	void play();
};

#endif