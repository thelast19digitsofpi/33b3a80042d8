// main.cpp
//


#include "console.h"
#include "util.h"
#include "parseRule.h"
#include "ParseError.h"
#include "Game.h"
#include "StringGenerator.h"

#include <iostream>
#include <random>
#include <chrono>
#include <thread>

// test stuff (not a formal Test)
void testEvaluation();
void testGame();

// for testing
bool hasTwoEs(std::string s) {
	int numEs = 0;
	for (char c: s) {
		if (c == 'E') {
			numEs++;
		}
	}
	
	return numEs >= 2;
}


int main() {
	myConsole::setup();
	setupRandom();
	
	testGame();
	
	std::cout << std::endl;
};


void testGame() {
	int minLevel, maxLevel;
	bool good = false;
	std::cout << "Welcome to Induction!\n";
	std::cout << "Please enter the difficulty level RANGE you wish to play (1 = easy, 5 = very hard)." << std::endl;
	// ask for user's input
	while (!good) {
		std::string s1 = myConsole::readInput("Lowest Level (1-5): ");
		if (std::cin.eof()) {
			return;
		}
		
		std::string s2 = myConsole::readInput("Highest Level (1-5): ");
		if (std::cin.eof()) {
			return;
		}
		
		try {
			minLevel = std::stoi(s1);
			maxLevel = std::stoi(s2);
			if (minLevel < 1 || minLevel > 5) {
				std::cerr << RED "Minimum level must be between 1 and 5.\n" BLACK << std::endl;
			} else if (maxLevel < 1 || maxLevel > 5) {
				std::cerr << RED "Maximum level must be between 1 and 5.\n" BLACK << std::endl;
			} else if (maxLevel < minLevel) {
				std::cout << "I assume you meant from " << maxLevel << " to " << minLevel << "?" << std::endl;
				// swaaaaaaaaap
				int temp = minLevel;
				minLevel = maxLevel;
				maxLevel = temp;
				good = true;
			} else {
				good = true;
			}
		} catch (std::invalid_argument& ex) {
			std::cerr << RED "That does not look like a number, please try again.\n" BLACK << std::endl;
		}
	}
	
	// Preparation for time delay
	auto wake = std::chrono::system_clock::now() + std::chrono::seconds(maxLevel);
	
	std::cout << "\nCreating Rule . . .\n" << std::endl;
	Game myGame(minLevel, maxLevel);
	// Just so you can't deduce the complexity of a rule by how long it spends verifying it
	// (although for level-5 rules it could be argued that it just wasted time on a bad rule?)
	std::this_thread::sleep_until(wake);
	
	myConsole::prepareExitReveal(&myGame); // so if you Ctrl+C out, it shows the rule
	// addExamples might be a better name?
	myGame.checkGuess(nullptr);
	myGame.printTable();
	
	std::cout << std::endl << "Make your first guess as to the rule..." << std::endl;
	
	bool keepGoing = true;
	while (keepGoing) {
		// string literals are concatenated automatically and the macros are string literals
		std::string s = myConsole::readInput("Enter your rule: ");
		if (std::cin.eof()) {
			break;
		}
		// this is cheating
		if (s == "cheatPrintRule") {
			myGame.printRule();
			std::cout << RED "You have officially cheated." BLACK << std::endl;
			continue;
		}
		MultiRule yourRule;
		try {
			parseRule(s, yourRule);
			std::cout << yourRule << std::endl;
		} catch (ParseError ex) {
			std::cout << RED << ex << BLACK << std::endl;
			continue;
		}
		std::cout << "\n\n" << std::endl;
		
		bool won = myGame.checkGuess(&yourRule);
		if (won) {
			keepGoing = false;
		} else {
			myGame.printTable();
		}
		
		std::cout << "Press Enter To Continue" << std::endl;
		getline(std::cin, s); // waste it
	}
	
	std::cout << "\n\nby the way, the rule was officially worded as:\n";
	myGame.printRule();
	std::cout << "\n(if for some reason those look different, that is okay, they do"
	             "\nindeed match for all strings in the game)\n" << std::endl;
	myConsole::prepareExitReveal(nullptr);
};


void testEvaluation() { 
	MultiRule rule;
	
	bool keepGoing = true;
	while (keepGoing) {
		std::string s = myConsole::readInput("Enter rule: ");
		try {
			parseRule(s, rule);
			std::cout << rule << std::endl;
			keepGoing = false;
		} catch (ParseError ex) {
			std::cout << RED << ex << BLACK << std::endl;
		}
		
		// if I clearly wish to stop, end the program
		if (s.empty() || std::cin.eof()) {
			return;
		}
	}
	
	// now the real test
	keepGoing = true;
	while (keepGoing) {
		std::string s = uppercase(myConsole::readInput("Enter word: "));
		if (s.empty() || std::cin.eof()) {
			keepGoing = false;
			break;
		}
		
		std::cout << "Your input " << s << " is a ";
		if (rule.evaluate(s)) {
			std::cout << GREEN << "POSITIVE";
		} else {
			std::cout << YELLOW << "NEGATIVE";
		}
		std::cout << BLACK << " word." << std::endl;
	}
}


