// Game.h
//
// For the table of which strings are positive and which are negative

#ifndef GAME_H_EXISTS
#define GAME_H_EXISTS

#include "MultiRule.h"

#include <string>
#include <set>

class Game {
	protected:
		// Hmmm, I didn't realize red-black trees could work for maps...
		std::set<std::string> positives;
		std::set<std::string> negatives;
		MultiRule actualRule;
		// Table has to know this in order to search for counterexamples
		// (it's a brute force search)
		int maxLength;
		int difficulty;
	public:
		Game(int, int);
		
		void addString(std::string, bool);
		bool checkGuess(MultiRule*); // true if you won; also adds counterexamples if not.
		void printTable();
		
		// prints the rule in its initial form
		void printRule();
		
		// prints information when you win
		void onWin();
		
		static MultiRule generateRule(int);
};

#endif
