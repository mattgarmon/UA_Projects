// (c) Matt Garmon - 2017
// card.hpp - Declaration of Card

#ifndef CARD
#define CARD

#include <iosfwd>

//rank enumeration
enum Rank {Ace, Two, Three, Four, Five, Six, Seven, 
	Eight, Nine, Ten, Jack, Queen, King,};

//suit enumeration
enum Suit {Clubs, Diamonds, Hearts, Spades,};

//card class (ranked)
class Card {
private:
	Rank rank; //rank member variable
	Suit suit; //suit member variable
public:
	Card(); //default constructor
	Card(Rank r, Suit s) : rank(r), suit(s) {} //parameterized constructor
	Rank get_rank() const {return rank;} //rank accessor
	Suit get_suit() const {return suit;} //suit accessor
	int get_value(); //card blackjack value
	int count_value(); //card counting value
};

//equality
bool operator==(Card a, Card b);
bool operator!=(Card a, Card b);

//ordering
bool operator<(Card a, Card b);
bool operator>(Card a, Card b);
bool operator<=(Card a, Card b);
bool operator>=(Card a, Card b);

//ostream operators
std::ostream& operator<<(std::ostream& os, Card c);
std::ostream& operator<<(std::ostream& os, Rank r);
std::ostream& operator<<(std::ostream& os, Suit s);

#endif
