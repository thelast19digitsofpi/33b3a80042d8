// PatternCount.cpp
//
// implementation of PatternCount


#include "PatternCount.h"

#ifdef DEBUG
#include <iostream>
#endif

// Neither constructor needs a body
// Given an existing PatternFinder and NumericCondition...
PatternCount::PatternCount(const PatternFinder& givenPattern, const NumericCondition& givenCondition)
: pattern(givenPattern), condition(givenCondition) {};
// ...or they can be made from a string, number, and operation
PatternCount::PatternCount(std::string patternString, int comparator, NumericOperator op)
: pattern(patternString), condition(comparator, op) {};
// ...or a character (this is helpful in my rule-build function)
PatternCount::PatternCount(char singleLetter, int comparator, NumericOperator op)
: pattern(false, std::string(1, singleLetter), false), condition(comparator, op) {};

// Evaluation: we already did the hard work here
bool PatternCount::evaluate(std::string string) {
	// C++ doesn't have many one-liners like Python does, but...
	return condition.evaluate(pattern.count(string));
};

void PatternCount::outputToStream(std::ostream& stream) const {
	stream << condition << " of " << pattern;
};