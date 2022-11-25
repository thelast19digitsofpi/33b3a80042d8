// PatternFinder.cpp
//
// I guess this is sort of like a regular expression

#include "PatternFinder.h"
#include "util.h"
#include "ParseError.h"

#include <utility>
#include <stack>

int countMatches(const char*, const char*, bool, bool) noexcept;

PatternFinder::PatternFinder(std::string givenPattern)
: mustStart(false), pattern(givenPattern), mustEnd(false) {
	if (pattern[0] == 'S') {
		// must start
		mustStart = true;
		pattern.erase(0, 1);
	}
	
	if (pattern.back() == 'F') {
		mustEnd = true;
		// that weird "reverse iterator" thing
		pattern.erase(pattern.length() - 1, 1);
	}
};

// this will never use the S/F thing
PatternFinder::PatternFinder(char givenLetter)
: mustStart(false), pattern(std::string(1, givenLetter)), mustEnd(false) {};

PatternFinder::PatternFinder(bool givenMustStart, std::string givenPattern, bool givenMustEnd) {
	mustStart = givenMustStart;
	mustEnd = givenMustEnd;
	pattern = givenPattern;
};

PatternFinder::PatternFinder(bool givenMustStart, const char* givenPattern, bool givenMustEnd) {
	mustStart = givenMustStart;
	mustEnd = givenMustEnd;
	pattern = std::string(givenPattern);
};

// Checks if a pattern is valid and THROWS if not
void PatternFinder::validatePattern(std::string pattern) {
	int len = pattern.length();
	for (int i = 0; i < len; i++) {
		char c = pattern[i];
		if (!isValidLetter(c) &&
			!(c >= '0' && c <= '9') &&
			c != '?' && c != '*') {
			throw ParseError("The character \"" + std::string(1, c) + "\" is not allowed in a pattern. You can use A, E, I, O, U, digits 0-9, and ?.");
		}
		
		if (c == '*' && pattern[i+1] == '*') {
			throw ParseError("The character \"*\" may not appear twice in a row. It already stands for a series of letters (you may be confusing it with \"?\").");
		}
		
		// S at the beginning marks a required start
		// S at the end was already removed just for convenience
		if (c == 'S' && i > 0) {
			throw ParseError("The letter S can only appear at the start of the pattern (and at the end, where it is ignored).");
		}
		if (c == 'F' && i < len-1) {
			throw ParseError("The letter F can only appear at the end of the pattern.");
		}
	}
};


// my algorithm works better with c-strings
int PatternFinder::count(std::string string) const {
	return countMatches(string.c_str(), pattern.c_str(), mustStart, mustEnd);
};

int PatternFinder::count(const char* string) const {
	return countMatches(string, pattern.c_str(), mustStart, mustEnd);
};

int countMatches(const char* string, const char* pattern, bool mustStart, bool mustEnd) noexcept {
	/*
	This function is mostly an exercise in writing recursive algorithms "by hand".
	The stuff inside the first for loop (in `si`), with the exception of the '*' clause, was written before I supported variable-length wildcards.
	*/
	int counted = 0;
	
	// standard null-terminated string loop
	char sc;
	for (int si = 0; (sc = string[si]); si++) {
		// check if the pattern exists starting here
		// numbers can stand for any specific letter, different digits must be different letters
		// (e.g. 12321 can stand for AUOUA but not AUAUA)
		bool good = true;
		char wildcards[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		// "pi" is pattern index, not 3.14159...
		int si2 = si;
		char pc;
		for (int pi = 0; (pc = pattern[pi]) && good; pi++) {
			if (pc >= '0' && pc <= '9') {
				// hit a wildcard, see if it is registered
				int digit = pc - '0';
				if (wildcards[digit]) {
					// this has already been registered, so this character must match
					good = (wildcards[digit] == string[si2]);
					// check if that one was already used
					for (int other = 0; other < 10; other++) {
						if (other != digit && wildcards[other] == wildcards[digit]) {
							good = false;
							break;
						}
					}
				} else {
					// this has not been registered
					wildcards[digit] = string[si2];
				}
			} else if (isValidLetter(pc)) {
				// direct match
				good = (string[si2] == pc);
			} else if (pc == '?') {
				// do nothing
			} else if (pc == '*') {
				// uh oh... recursion time!
				// note: * can match zero or more letters
				counted += countMatches(
					string + si2, // if this were a letter then string[si2] would match this letter
					pattern + pi + 1, // remove the asterisk
					false, // we don't need it to start there, that's the whole point of the *
					mustEnd
				);
				good = false; // don't make it claim one extra
			}
			
			si2++;
		}
		
		// if it's actually a match, and we aren't mid-string when we can only match the end...
		// (this is needed because I plan to support variable-length wildcards)
		if (good && !(mustEnd && string[si2] != '\0')) {
			counted++;
		}
		
		// must-start should never have i > 0
		if (mustStart) {
			break;
		}
	}
	
	return counted;
};

void PatternFinder::outputToStream(std::ostream& stream) const {
	if (mustStart) {
		stream << 's';
	}
	stream << pattern;
	if (mustEnd) {
		stream << 'f';
	}
};
