// parseRule.cpp
//
// Now the REAL work.

#include "util.h"
#include "parseRule.h"
#include "PatternCount.h"
#include "NumericCondition.h"

#include <sstream>
#include <algorithm>

#ifdef DEBUG
#include <iostream>
#endif

// Internal functions
void parseLayer(std::stringstream&, MultiRule&, std::string);
void giveBackParentheses(std::stringstream&, std::string&);
PatternCount parseCountPattern(std::stringstream&, NumericOperator);
PatternCount parseCountPattern(std::stringstream&, NumericOperator, int);
PatternCompare parseComparePattern(std::stringstream&, NumericOperator);
PatternCount parseParityPattern(std::stringstream&, std::string);
void handleAtomicRule(std::stringstream&, MultiRule&, std::string);

// Throws ParseError on invalid string
void parseRule(const char* cString, MultiRule& container) {
	parseRule(std::string(cString), container);
};

// Throws ParseError on invalid string. Otherwise, modifies the MultiRule.
void parseRule(std::string string, MultiRule& container) {
	// if the length is zero then dont return anything.
	int length = string.length();
	if (length == 0) {
		throw ParseError("Nothing entered.");
	}
	// initialize our reading stream and some generic strings
	std::stringstream ss(uppercase(string));
	std::string word1, word2;
	
	// read to the first space
	getline(ss, word1, ' ');
	#ifdef DEBUG
	std::cout << BLUE << "Parsing:" << BLACK << " " << word1 << std::endl;
	#endif
	
	parseLayer(ss, container, word1);
};

void setOperation(MultiRule& container, std::string word1) {
	BooleanOperator op;
	if (word1 == "AND") {
		op = ALL;
	} else if (word1 == "OR") {
		op = SOME;
	} else {
		std::cerr << "\n\n" << std::endl << word1;
		throw ParseError("setOperation(): This is \"impossible\" (i.e. a bug)... please report this (and the word that appeared, if any)");
	}
	
	container.setOp(op);
};

// assuming the string reference makes it faster (and slightly easier to debug)
void parseLayer(std::stringstream& ss, MultiRule& container, std::string word1) {
	//std::cout << "entering layer with " << word1 << std::endl;
	bool keepGoing = true;
	while (keepGoing) {
		if (word1[0] == '(') {
			/*
			so basically, we can have rules like:
			(at least 1 a AND at least 1 e)
			(at least 1 a OR at least 1 e OR at least 1 i)
			
			we first need to parse a single rule, but without the first character.
			
			of course... it could in theory be another parenthesis...
			
			within each layer:
			- look at the first character of the current word.
			- if '(', add a new layer.
			- otherwise, attempt to parse an atomic rule, since that is the only thing we can parse at this stage.
			- parse 1 atomic rule (which, if you ended with a ')', will give it back to you)
			- parse the next word
			- if AND/OR, then set the operator (if it already was set to something else, throw)
			- if ), then pop this layer (i.e. add the inner MultiRule to the outer, then return)
			- if EOF, pop this layer, which will cause it to keep reading EOF and pop all layers
			- if 
			
			*/
			MultiRule inside;
			parseLayer(ss, inside, word1.substr(1, std::string::npos));
			container.addRule(inside);
		} else if (word1.empty() || word1 == " ") {
			// do nothing, this is just an artifact from parenthesis parsing
		} else if (word1 == "AND" || word1 == "OR") {
			setOperation(container, word1);
		} else {
			// it's a collection of simple rules
			handleAtomicRule(ss, container, word1);
		}
		
		getline(ss, word1, ' ');
		// if we hit end of file we are done (there can't be any content that is a single word)
		if (ss.eof()) {
			keepGoing = false;
		}
		
		// handleAtomicRule kindly gives us our parentheses back
		if (word1[0] == ')') {
			// 1 whitespace and any excessive parentheses
			for (int i = word1.size(); i > 0; i--) {
				ss.unget();
			}
			keepGoing = false;
		}
	}
	
	// if we walked out with only 1 rule then set the flag
	if (container.getTotalRules() == 1 && container.getOp() == BOOLEAN_UNDEFINED) {
		// a single rule, put an operation on it to prevent hassles
		container.setOp(SOME);
	}
	
	//std::cout << "exiting layer" << std::endl;
};

// Makes a count pattern or a compare pattern ("atomic" as they do not contain other rules)
void handleAtomicRule(std::stringstream& ss, MultiRule& container, std::string word1) {
	std::string word2;
	NumericOperator opNum;
	//BooleanOperator opBool;
	if (word1 == "AT") {
		// ok, what's the NEXT word?
		getline(ss, word2, ' ');
		if (ss.eof()) {
			throw ParseError("Looks like you ended your rule with \"at\", \"at least\", or \"at most\". What are you referring to?");
		} else if (word2 == "LEAST") {
			opNum = GREATER_EQUAL;
		} else if (word2 == "MOST") {
			opNum = LESS_EQUAL;
		} else {
			// you failed
			std::stringstream error;
			error << "Looks like you tried to put \"at least\" or \"at most\", but wrote ";
			error << word2;
			error << " instead!";
			throw ParseError(error.str());
		}
		
		container.addRule(parseCountPattern(ss, opNum));
	} else if (word1 == "EXACTLY") {
		container.addRule(parseCountPattern(ss, EQUAL));
	} else if (word1 == "SOME" || word1 == "ANY") {
		container.addRule(parseCountPattern(ss, GREATER_EQUAL, 1));
	} else if (word1 == "NONE" || word1 == "NO") {
		// hmmm this conflicts with using "none of" as a MultiRule operator
		// I guess I can use "neither" and "not"
		container.addRule(parseCountPattern(ss, EQUAL, 0));
	} else if (word1 == "AN") {
		// parity checks, look for "odd" or "even"
		getline(ss, word2, ' ');
		if (ss.eof()) {
			throw ParseError("Looks like you ended your rule with \"an\". What are you referring to?");
		}
		if (word2 == "ODD" || word2 == "EVEN") {
			container.addRule(parseParityPattern(ss, word2));
		} else {
			throw ParseError("Looks like you entered the word \"an\" but didn't follow it with ODD or EVEN. You cannot write \"an O\"; you would have to put \"exactly 1 O\" (or at least 1 O).");
		}
	} else if (word1 == "AS") {
		// as many X as Y
		getline(ss, word2, ' ');
		if (ss.eof()) {
			throw ParseError("Looks like you ended your rule with \"as\". What are you referring to?");
		}
		if (word2 == "MANY" || word2 == "MUCH") {
			container.addRule(parseComparePattern(ss, EQUAL));
		} else {
			throw ParseError("I am not sure what to do with " + word2 + " after the word \"AS\". It could be something else is badly formed.");
		}
	} else if (word1 == "MORE") {
		container.addRule(parseComparePattern(ss, GREATER));
	} else if (word1 == "LESS" || word1 == "FEWER") { // accept both
		container.addRule(parseComparePattern(ss, LESS));
	} else {
		throw ParseError("I found the word \"" + word1 + "\", but I don't know what to do with it.");
	}
};

// These are slightly different
PatternCount parseParityPattern(std::stringstream& ss, std::string word2) {
	// here the operator is PARITY but the comparison number is different
	int num;
	if (word2 == "ODD") {
		num = 1;
	} else if (word2 == "EVEN") {
		num = 2;
	} else {
		throw ParseError("...this is impossible (i.e. a bug)... your rule had something to do with parity?");
	}
	
	std::string word3, word4, word5;
	
	getline(ss, word3, ' '); // number
	getline(ss, word4, ' '); // of
	if (ss.eof()) { // it might eof just after parsing the last word
		throw ParseError("For odd/even rules, you must write out the full \"an odd number of PATTERN\" (or even).");
	}
	getline(ss, word5, ' '); // <pattern>
	
	// darn, I'm reusing code again
	giveBackParentheses(ss, word5);
	
	return PatternCount(word5, num, PARITY);
};


// Assumes the operator has already been read off but the number has not
PatternCount parseCountPattern(std::stringstream& ss, NumericOperator op) {
	#ifdef DEBUG
	std::cout << "looking at " << (char)(ss.peek()) << std::endl;
	#endif
	
	std::string word;
	int number = -1;
	// exception handling is always messy
	try {
		getline(ss, word, ' ');
		number = stoi(word, nullptr, 10);
	} catch (std::invalid_argument& exc) {
		// re-throw as a parse error
		throw ParseError("Invalid number... looks like you put \"at least\" or similar and either forgot the number or put something else before it.");
	}
	
	if (ss.eof()) {
		std::stringstream error;
		error << "Uh, " << number << " of what? You have a number but I don't see a pattern.";
		throw ParseError(error.str());
	}
	
	return parseCountPattern(ss, op, number);
};

// I split the functions because of the "some" shortcut
PatternCount parseCountPattern(std::stringstream& ss, NumericOperator op, int number) {
	std::string word;
	getline(ss, word, ' ');
	// "exactly 1 E" and "exactly 1 of E" are both allowed
	if (word == "OF") {
		getline(ss, word, ' ');
		if (word == "OF") {
			throw ParseError("You seem to have ended your rule with the word \"of\"...");
		}
	}
	
	giveBackParentheses(ss, word);
	
	// remove trailing S
	int len = word.length();
	if (word[len-1] == 'S') {
		word.resize(len-1);
		len--;
	}
	
	if (word == " " || word.empty()) {
		std::stringstream error;
		error << "Uh, " << number << " of what? You have a number but I don't see a pattern to match.";
		throw ParseError(error.str());
	}
	
	// check for validity
	for (int i = 0; i < len; i++) {
		char c = word[i];
		if (!isValidLetter(c) &&
			!(c >= '0' && c <= '9') &&
			c != '?' && c != '*' && c != 'S' && c != 'F') {
			throw ParseError("The character \"" + std::string(1, c) + "\" is not allowed in a pattern. You can use A, E, I, O, U, digits 0-9, and ?.");
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

	//std::cout << BLUE "DEBUG: " BLACK << "Number: " << number << ", Word: " << word << ", Operation: " << (char)op << std::endl;
	
	return PatternCount(word, number, op);
};

// also assumes the operator was already read off
PatternCompare parseComparePattern(std::stringstream& ss, NumericOperator op) {
	std::string pattern1, innerWord, pattern2;
	getline(ss, pattern1, ' ');
	getline(ss, innerWord, ' ');
	if (innerWord != "THAN" && innerWord != "AS") {
		// this could mean another error so we throw
		// e.g. more E I could cause problems parsing later on
		throw ParseError("You were comparing two patterns " +
			(op == EQUAL ? std::string("(as many X as Y)") : std::string("(more/fewer X than Y)")) +
			", but instead of \"than\" or \"as\", I found the word \"" + innerWord + "\" in the middle. This could be an error so I ask you to correct your input.");
	}
	if (ss.eof()) {
		throw ParseError("You were comparing two patterns (as many, more, or fewer X than Y) but you forgot the second one.");
	}
	
	getline(ss, pattern2, ' ');
	if (pattern2.empty()) {
		throw ParseError("You were comparing two patterns but seem to have forgotten the second one. Maybe there is some other mistake?");
	}
	
	// wow, glad I caught this...
	giveBackParentheses(ss, pattern2);
	
	return PatternCompare(pattern1, pattern2, op);
};

// the string is a reference because we mutate it
void giveBackParentheses(std::stringstream& ss, std::string& word) {
	// darn, I'm reusing code again
	int length = word.size();
	// ignore any parentheses
	if (word[length - 1] == ')') {
		// put the space and parenthesis back
		ss.unget();
		ss.unget();
		word.erase(length - 1, 1);
		// if there are multiple parens, put all of them back
		for (int i = length - 2; i >= 0; i--) {
			if (word[i] == ')') {
				ss.unget();
				word.erase(i, 1);
			} else {
				break;
			}
		}
	}
};
