// console.cpp
//
// so that I can decide not to ever touch linenoise again


#include <iostream>
#include "console.h"
#include <cstdlib>
#include "Game.h"

namespace myConsole {
	Game* currentGamePointer = nullptr;
	
	void setup() {
		// should be enough
		linenoiseHistorySetMaxLen(25);
	};
	
	void prepareExitReveal(Game* ptr) {
		currentGamePointer = ptr;
	};
	
	std::string readInput(const char* prompt) {
		char* rawInput = linenoise(prompt);
		if (rawInput == NULL) {
			if (currentGamePointer != nullptr) {
				std::cout << "The rule was:" << std::endl;
				currentGamePointer->printRule();
				currentGamePointer = nullptr;
			}
			std::cout << "\n\nGoodbye." << std::endl;
			exit(0);
		}
		linenoiseHistoryAdd(rawInput);
		std::string input(rawInput);
		linenoiseFree(rawInput);
		return input;
	};

	// string version
	std::string readInput(std::string prompt) {
		return readInput(prompt.c_str());
	};
};