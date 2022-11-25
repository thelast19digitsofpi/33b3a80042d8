// Game.cpp
//
// Despite its understated appearance...

#include "Game.h"
#include "util.h"
#include "NumericCondition.h"
#include "StringGenerator.h"

#include <iomanip>
#include <sstream>


Game::Game(int minLevel, int maxLevel) {
	// make sure the order is correct
	if (minLevel > maxLevel) {
		int temp = minLevel;
		minLevel = maxLevel;
		maxLevel = temp;
	}
	
	if (maxLevel > 5) {
		maxLevel = 5;
	}
	// set the maximum length based on the highest possible level: 1-3 is six, 4-5 is eight
	maxLength = (maxLevel > 3) ? 8 : 6;
	
	bool done = false;
	MultiRule possibleRule;
	difficulty = 0;
	do {
		difficulty = minLevel + getRandom(maxLevel - minLevel + 1);
		possibleRule = Game::generateRule(difficulty);
		// check that it has some positives and some negatives
		StringGenerator generator(maxLength);
		bool hasPositive = false, hasNegative = false;
		// Loop until it runs out of words OR has both a positive and a negative word found
		while (!generator.isDone() && !(hasPositive && hasNegative)) {
			// get the next word and test it
			std::string word = generator.next();
			if (possibleRule.evaluate(word)) {
				hasPositive = true;
			} else {
				hasNegative = true;
			}
		}
		
		if (hasPositive && hasNegative) {
			done = true;
			actualRule = possibleRule;
		}
	} while (!done);
};

void Game::addString(std::string word, bool isPositive) {
	if (isPositive) {
		positives.insert(word);
	} else {
		negatives.insert(word);
	}
};

// private function
int fiveToTheN(int n) noexcept {
	int answer = 1;
	for (int i = 0; i < n; i++) {
		answer *= 5;
	}
	return answer;
};

// This one is interesting.
// Null pointer = add 2 random examples (equivalent to a rule that always answers wrongly)
bool Game::checkGuess(MultiRule* rulePtr) {
	// step 1: check if the rule is confirmed false
	if (rulePtr) {
		for (std::string existingPositive: positives) {
			// If it evaluates to false on your rule, it must be wrong.
			if (!rulePtr->evaluate(existingPositive)) {
				std::cout << RED "Invalid Guess" BLACK ": the word \"" << existingPositive << "\" is a known " GREEN "POSITIVE" BLACK ", but your rule marks it as negative." << std::endl;
				return false; // you clearly didn't win
			}
		}
		for (std::string existingNegative: negatives) {
			// This time it needs to evaluate true.
			if (rulePtr->evaluate(existingNegative)) {
				std::cout << RED "Invalid Guess" BLACK ": the word \"" << existingNegative << "\" is a known " YELLOW "NEGATIVE" BLACK ", but your rule marks it as positive." << std::endl;
				return false;
			}
		}
	}
	
	// ok here we go
	// we create separate vectors for each string length
	// "false positive" means "labeled positive, but really it is negative"
	// +1 just to avoid index hassles
	std::vector<std::string> falseNegatives[maxLength + 1];
	std::vector<std::string> falsePositives[maxLength + 1];
	
	StringGenerator generator(maxLength);
	while (!generator.isDone()) {
		std::string word = generator.next();
		int len = word.length();
		// ok, check that string
		// see if your rule matches the real truth
		bool reallyPositive = actualRule.evaluate(word);
		bool markedPositive = (rulePtr == nullptr ? !reallyPositive : rulePtr->evaluate(word));
		// check if they marked it wrongly
		if (markedPositive && !reallyPositive) {
			falsePositives[len].push_back(std::string(word));
		} else if (!markedPositive && reallyPositive) {
			falseNegatives[len].push_back(std::string(word));
		}
	}
	
	// we can't do falseNegatives.size() because falseNegatives is an array of vectors
	int numFN = 0;
	int numFP = 0;
	
	// check which ones are populated
	std::vector<int> fnLengths;
	std::vector<int> fpLengths;
	for (int len = 1; len <= maxLength; len++) {
		// basically, if the false-positive vector for length 3 is populated then push 3 into the list of lengths
		int size = falsePositives[len].size();
		if (size > 0) {
			numFP += size;
			fpLengths.push_back(len);
		}
		size = falseNegatives[len].size();
		if (size > 0) {
			numFN += size;
			fnLengths.push_back(len);
		}
	}
	
	// Now we pick a random positive word that was mislabeled as negative, and display it.
	
	if (numFN > 0) {
		if (numFP == 0) {
			std::cout << "Your rule got all the true positive words correct!\nHowever, there are more positives." << std::endl;
		}
		
		// pick one at random
		int index1 = getRandom(fnLengths.size());
		int len = fnLengths[index1];
		int index2 = getRandom(falseNegatives[len].size());
		// false negative, remember, means it is really a positive
		std::string newPositive = falseNegatives[len][index2];
		
		if (rulePtr != nullptr) {
			std::cout << "The word " << GREEN << newPositive << BLACK << " is actually a " << GREEN << "POSITIVE" << BLACK << ",\nbut your rule thinks it is negative." << std::endl;
		}
		// it goes into the table
		positives.insert(newPositive);
	}
	if (numFP > 0) {
		if (numFN == 0) {
			std::cout << "Your rule did not miss any positives!\nHowever, it did overshoot and include some negatives." << std::endl;
		}
		
		// pick one at random
		int index1 = getRandom(fpLengths.size());
		int len = fpLengths[index1];
		int index2 = getRandom(falsePositives[len].size());
		std::string newNegative = falsePositives[len][index2];
		if (rulePtr != nullptr) {
			std::cout << "The word " << YELLOW << newNegative << BLACK << " is actually a " << YELLOW << "NEGATIVE" << BLACK << ",\nbut your rule thinks it is positive." << std::endl;
		}
		negatives.insert(newNegative);
	}
	
	// Return true if you won, i.e. no false anythings!
	bool won = (numFP == 0 && numFN == 0);
	if (won) {
		onWin();
	}
	return won;
};


void Game::printTable() {
	// prints all words discovered so far
	auto i1 = positives.begin();
	auto i2 = negatives.begin();
	
	int width = (maxLength < 8 ? 8 : maxLength);
	
	bool positivesGoing = (i1 != positives.end());
	bool negativesGoing = (i2 != negatives.end());
	
	std::cout << GREEN << std::setw(width) << std::right;
	std::cout << "Positive";
	std::cout << BLACK << " | ";
	std::cout << YELLOW << "Negative" << BLACK << std::endl;
	
	// line of hyphens
	// this is a performance-optimizing loop design, not that it's really useful but more for show
	for (int i = width*2 + 3; i > 0; i--) {
		std::cout << '-';
	}
	std::cout << std::endl;
	
	while (positivesGoing || negativesGoing) {
		std::cout << GREEN << std::setw(width) << std::left;
		if (positivesGoing) {
			// print and then advance
			std::cout << *i1;
			i1++;
		} else {
			std::cout << ' ';
		}
		std::cout << BLACK << " | ";
		std::cout << YELLOW << std::setw(width) << std::left;
		if (negativesGoing) {
			// print and then advance, again
			std::cout << *i2;
			i2++;
		} else {
			std::cout << ' ';
		}
		std::cout << BLACK << std::endl;
		positivesGoing = (i1 != positives.end());
		negativesGoing = (i2 != negatives.end());
	}
	
	std::cout << std::setw(0);
};

// for debug or for the end
void Game::printRule() {
	std::cout << actualRule << std::endl;
};

void Game::onWin() {
	std::cout << GREEN << std::endl <<
		"Y   Y  OOO  U   U   W   W III N   N !\n"
		" Y Y  O   O U   U   W W W  I  NN  N !\n"
		"  Y   O   O U   U   W W W  I  N N N !\n"
		"  Y   O   O U   U   W W W  I  N  NN  \n"
		"  Y    OOO   UUU     W W  III N   N !\n"
	<< BLACK << std::endl;
	
	// subtract 2 for the initial examples
	std::cout << BLUE << "Your Score: " << (positives.size() + negatives.size() - 2) << " on level " << difficulty << std::endl;
	std::cout << BLACK "(Lower score is better.)" << std::endl;
};


MultiRule Game::generateRule(int difficulty) {
	// 6 so as not to worry about the null character
	char alphabet[6] = "AEIOU";
	
	char scrambledAlphabet[6] = "AEIOU";
	// Shuffle algorithm
	for (int i = 4; i > 0; i--) {
		// swap random element into position i
		int j = getRandom(i + 1);
		if (j != i) {
			// swaaaaaaaaaap
			char temp = scrambledAlphabet[i];
			scrambledAlphabet[i] = scrambledAlphabet[j];
			scrambledAlphabet[j] = temp;
		}
	}
	
	//NumericOperator allOps[] = {LESS, LESS_EQUAL, EQUAL, GREATER_EQUAL, GREATER}; didn't need...
	NumericOperator fewerOps[] = {LESS_EQUAL, EQUAL, GREATER_EQUAL};
	
	if (difficulty == 1) {
		// simple rule
		int choice = getRandom(2);
		
		if (choice == 0) {
			// option 1: some quantity of a given letter
			// operations (the LESS and GREATER are redundant)
			int index = getRandom() % 3;
			NumericOperator op = fewerOps[index];
			// one-letter pattern
			index = getRandom(5);
			std::string pattern(1, alphabet[index]);
			// and then ask for either 1, 2, or 3 of it
			return MultiRule(PatternCount(pattern, 1 + getRandom() % 3, op));
		} else {
			// option 2: a two-character string, which must exist at least once
			// note that asking for a doubled letter is definitely possible
			char word[3] = {alphabet[getRandom(5)], alphabet[getRandom(5)], '\0'};
			return MultiRule(PatternCount(word, 0, GREATER));
		}
	} else if (difficulty == 2) {
		// total rules: 20 + (10 + 10 + 20) + (10 + 10) + 2 = 82
		// more complicated: involving more <letter> than <letter> or a simple AND/OR
		// or, rarely, must have (or not have) a doubled letter
		int choice = getRandom(100);
		if (choice < 25) { // 25%
			// option 1: more of one letter than another
			// turn the characters into one-length strings
			return MultiRule(PatternCompare(
				std::string(1, scrambledAlphabet[0]),
				std::string(1, scrambledAlphabet[1]),
				GREATER
			));
		} else if (choice < 65) { // most common (40%)
			// simple AND/OR
			// 0 = X or Y, 1 = X and Y, 2 = X and not Y (done as some X and no Y)
			int opChoice = getRandom(3);
			BooleanOperator op = opChoice == 0 ? SOME : ALL;
			MultiRule rule(op);
			rule.addRule(PatternCount(std::string(1, scrambledAlphabet[0]), 1, GREATER_EQUAL));
			if (opChoice == 2) {
				// the second rule is an exclusion, so we don't have to add more layers of MultiRules
				rule.addRule(PatternCount(std::string(1, scrambledAlphabet[1]), 0, EQUAL));
			} else {
				rule.addRule(PatternCount(std::string(1, scrambledAlphabet[1]), 1, GREATER_EQUAL));
			}
			return rule;
		} else if (choice < 90) { // also 25%
			// either XY or YX, or neither XY nor YX
			int opChoice = getRandom(2);
			MultiRule rule(opChoice == 1 ? SOME : ALL);
			char order1[3] = {scrambledAlphabet[0], scrambledAlphabet[1], 0};
			char order2[3] = {scrambledAlphabet[1], scrambledAlphabet[0], 0};
			rule.addRule(PatternCount(std::string(order1), 0, opChoice == 1 ? GREATER : EQUAL));
			rule.addRule(PatternCount(std::string(order2), 0, opChoice == 1 ? GREATER : EQUAL));
			return rule;
		} else { // rare (10%) because it only generates 2 rules
			// either must have a doubled letter (AEEI does but AEIE does not) or must not
			return MultiRule(PatternCount("11", 0, getRandom(2) ? EQUAL : GREATER));
		}
	} else if (difficulty == 3) {
		// now we introduce more complicated things (365 total rules?)
		/*
		Commas denote separate options while non-comma or denotes a compound rule.
		8 options (X,Y,Z are different letters):
		(0) More of XY than XZ, or more of YX than ZX [120]
		(1) At least 1 XY or at least 1 XZ, or the mirror image of such [60]
		(2) An odd number, or an even number, of X [10]
		(3) between 1 and 2, or between 2 and 3, or between 1 and 3, of X [15]
		(4) contains X and Y but not Z, or contains X but neither Y nor Z [60]
		(5) more XY than YX [20]
		(6) more X than Y and more Y than Z [60]
		(7) some X strictly before a Y (realized with the pattern X*Y) [20]
		*/
		int choice = getRandom(8);
		if (choice == 0 || choice == 1) {
			// more instances of 12 than 13, or 21 vs 31, where each digit is a letter
			// 0s for null-terminated strings
			char pattern1[3] = {scrambledAlphabet[0], scrambledAlphabet[1], 0};
			char pattern2[3] = {0, 0, 0};
			if (getRandom(2)) {
				// e.g. AE vs AI
				pattern2[0] = scrambledAlphabet[0];
				pattern2[1] = scrambledAlphabet[2];
			} else {
				// e.g. AE vs IE
				pattern2[0] = scrambledAlphabet[2];
				pattern2[1] = scrambledAlphabet[1];
			}
			
			if (choice == 0) {
				return MultiRule(PatternCompare(pattern1, pattern2, GREATER));
			} else {
				// do a "must have either AE or AI" rule
				MultiRule rule(SOME);
				rule.addRule(PatternCount(pattern1, 1, GREATER_EQUAL));
				rule.addRule(PatternCount(pattern2, 1, GREATER_EQUAL));
				return rule;
			}
		} else if (choice == 2) {
			// parity rules: odd/even number of a specific letter
			return MultiRule(PatternCount(std::string(1, scrambledAlphabet[0]), getRandom(2), PARITY));
		} else if (choice == 3) {
			// between 1 and 2, 2 and 3, or 1 and 3 of a single letter
			MultiRule rule(ALL);
			int choice2 = getRandom(3);
			// choice: 0, 1, 2
			// low:    1, 1, 2
			// high:   2, 3, 3
			int low = 1 + choice2/2;
			int high = 2 + (choice2 + 1)/2;
			// either [1,2], [1,2,3], or [2,3]
			// [1,2,3] nicely crashes into the odd/even one a bit...
			rule.addRule(PatternCount(scrambledAlphabet[0], low, GREATER_EQUAL));
			rule.addRule(PatternCount(scrambledAlphabet[0], high, LESS_EQUAL));
			
			return rule;
		} else if (choice == 4) {
			// 2 options: (1 and 2 and not 3), (1 and not 2 and not 3)
			MultiRule rule(ALL);
			rule.addRule(PatternCount(scrambledAlphabet[0], 1, GREATER_EQUAL));
			// less than 1 means none
			rule.addRule(PatternCount(scrambledAlphabet[1], 1, getRandom(2) ? LESS : GREATER_EQUAL));
			rule.addRule(PatternCount(scrambledAlphabet[2], 1, LESS));
			
			return rule;
		} else if (choice == 5) {
			// more XY than YX
			char pattern1[3] = {scrambledAlphabet[0], scrambledAlphabet[1], '\0'};
			char pattern2[3] = {scrambledAlphabet[1], scrambledAlphabet[0], '\0'};
			return MultiRule(PatternCompare(pattern1, pattern2, GREATER));
		} else if (choice == 6) {
			MultiRule rule(ALL);
			// X > Y and Y > Z
			rule.addRule(PatternCompare(scrambledAlphabet[0], scrambledAlphabet[1], GREATER));
			rule.addRule(PatternCompare(scrambledAlphabet[1], scrambledAlphabet[2], GREATER));
			return rule;
		} else if (choice == 7) {
			// pattern X*Y looks for an X, then any number of characters, then a Y
			char pattern[4] = {scrambledAlphabet[0], '*', scrambledAlphabet[1], '\0'};
			return MultiRule(PatternCount(pattern, 1, GREATER_EQUAL));
		}
	} else if (difficulty == 4) {
		/*
		(0) Logical combination (AND/OR) of two rules in the 1-2 range
		(1) ^
		(2) ^^
		(3) more W than X or more Y than Z
		(4) contains X and Y at a distance of 2 or 3 (realized as "some X??Y or some Y??X")
		(5) some X before a Y but no Z in between (realized as "some X*Y and no X*Z*Y")
		
		It looks like (0-2) gives 11628 different rules. Of course some of them probably collapse into one another or break entirely...
		
		(3) gives 60, (4) gives 20, (5) gives 60.
		
		But there's really no point in counting.
		*/
		int choice = getRandom(6);
		if (choice <= 2) {
			// The game will check only if the rule has some positives and some negatives
			MultiRule rule(getRandom(2) ? ALL : SOME);
			// add a random level 1 or level 2 rule, twice
			rule.addRule(Game::generateRule(getRandom(2) + 1));
			rule.addRule(Game::generateRule(getRandom(2) + 1));
			
			// it's entirely possible that one rule eclipses the other e.g. "at least 1 A and more A than E"... but if you're expecting a level 4 rule that might throw you off...
			// I mean, one time it took me 5 turns to guess "exactly 0 AE", a level 1 rule (now removed) when I usually guess those in 2 or 3, because I forgot it *was* a level 1.
			return rule;
		} else if (choice == 3) {
			MultiRule rule(getRandom(2) ? ALL : SOME); // logical OR
			// W > X or Y > Z
			rule.addRule(PatternCompare(scrambledAlphabet[0], scrambledAlphabet[1], GREATER));
			rule.addRule(PatternCompare(scrambledAlphabet[2], scrambledAlphabet[3], GREATER));
			return rule;
		} else if (choice == 4) {
			MultiRule rule(SOME); // logical OR
			std::stringstream pattern;
			pattern << scrambledAlphabet[0];
			// add 1-3 ?'s
			int numSpaces = getRandom(2) + 1;
			for (int i = 0; i < numSpaces; i++) {
				pattern << '?';
			}
			pattern << scrambledAlphabet[1];
			
			// now make the rule
			std::string patternString = pattern.str();
			rule.addRule(PatternCount(patternString, 1, GREATER_EQUAL));
			// now we switch the two ends
			patternString[0] = scrambledAlphabet[1];
			patternString[numSpaces + 1] = scrambledAlphabet[0];
			// this is ok because it copies it into a new string
			rule.addRule(PatternCount(patternString, 1, GREATER_EQUAL));
			return rule;
		} else if (choice == 5) {
			MultiRule rule(ALL); // logical AND
			char pattern1[4] = {scrambledAlphabet[0], '*', scrambledAlphabet[2], '\0'};
			char pattern2[6] = {scrambledAlphabet[0], '*', scrambledAlphabet[1], '*', scrambledAlphabet[2], '\0'};
			// classic this-but-not-that construction
			rule.addRule(PatternCount(pattern1, 1, GREATER_EQUAL));
			rule.addRule(PatternCount(pattern2, 0, EQUAL));
			return rule;
		}
	} else if (difficulty == 5) {
		// even more highly experimental
		// makes up to 4 random rules with total complexity 6 (or possibly 4-5)
		// at this point, counting the number of rules is absolutely hopeless but I'm fairly certain it is over a million
		MultiRule rule(getRandom(2) ? ALL : SOME);
		int complexityLeft = 6;
		while (complexityLeft > 0 && rule.getTotalRules() < 4) {
			// pick a random from 1 to 4
			int level = getRandom(4) + 1;
			if (level > complexityLeft) {
				level = complexityLeft;
			}
			// add a rule of that level
			rule.addRule(Game::generateRule(level));
			complexityLeft -= level;
			//std::cout << "making a level " << level << " rule" << std::endl;
		}
		
		return rule;
	}
	
	throw "nope";
};
