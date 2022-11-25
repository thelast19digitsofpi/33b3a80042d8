// console.h
//
// defines my wrapper around readline

#ifndef CONSOLE_H_EXISTS
#define CONSOLE_H_EXISTS

#include <string>
#include "linenoise/linenoise.h"
#include "Game.h"

// using a namespace
// another of those "good practice but not really needed here" things
namespace myConsole {

	void setup();
	
	// so it reveals the rule
	void prepareExitReveal(Game*);

	std::string readInput(const char* prompt);
	std::string readInput(std::string prompt);

};

#endif
