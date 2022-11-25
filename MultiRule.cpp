// MultiRule.cpp
//
// Nothing remarkable here

#include "MultiRule.h"


// should let the vector initialize itself
MultiRule::MultiRule()
: op(BOOLEAN_UNDEFINED) {};

MultiRule::MultiRule(BooleanOperator givenOp)
: op(givenOp) {};

// For more complicated rules you are supposed to build them using addRule
// but because MultiRule is a return type, we have to do this:
MultiRule::MultiRule(PatternCount&& pc)
: op(SOME) {
	basicRules.push_back(pc);
	totalRules = 1;
};
MultiRule::MultiRule(PatternCompare&& pc)
: op(SOME) {
	compareRules.push_back(pc);
	totalRules = 1;
};

// Constructing from another MultiRule just copies it which is needed for C++ to work

// not sure if this does anything
MultiRule::~MultiRule() {};


// getters and setters
BooleanOperator MultiRule::getOp() {
	return op;
};
void MultiRule::setOp(BooleanOperator newOp) {
	op = newOp;
};

int MultiRule::getTotalRules() const {
	return totalRules;
};

void MultiRule::addRule(PatternCount rule) {
	basicRules.push_back(rule);
	totalRules++;
};
void MultiRule::addRule(PatternCompare rule) {
	compareRules.push_back(rule);
	totalRules++;
};
void MultiRule::addRule(MultiRule rule) {
	multiRules.push_back(rule);
	totalRules++;
};

bool MultiRule::evaluate(std::string word) {
	int matched = 0;
	#define EVALUATE_LOOP(TYPE, rules) for (TYPE rule: rules) { \
		if (rule.evaluate(word)) { \
			matched++; \
		} \
	}
	
	EVALUATE_LOOP(PatternCount, basicRules)
	EVALUATE_LOOP(PatternCompare, compareRules)
	EVALUATE_LOOP(MultiRule, multiRules)
	
	switch (op) {
		case BOOLEAN_UNDEFINED:
			return false;
		case NONE:
			return matched == 0;
		case ONLY_ONE:
			return matched == 1;
		case SOME:
			return matched >= 1;
		case ALL:
			return matched == totalRules;
		default:
			return false;
	}
};

// I think I am very inconsistent with this parameter name...
void MultiRule::outputToStream(std::ostream& stream) const {
	const char* separator = "; ";
	if (totalRules >= 2) {
		switch (op) {
			case BOOLEAN_UNDEFINED:
				stream << "[malformed rule]";
				return;
			case NONE:
				stream << "none of (";
				break;
			case ONLY_ONE:
				stream << "exactly one of (";
				break;
			case SOME:
				stream << "(";
				separator = " OR ";
				break;
			case ALL:
				stream << "(";
				separator = " AND ";
				break;
		}
	} else {
		if (op == NONE) {
			stream << "NOT (";
		}
	}
	
	bool addSeparator = false;

	// this is unfortunate but eh
	#define SEPARATOR_CHECK if (addSeparator) { \
		stream << separator; \
	} else { \
		addSeparator = true; \
	} \

	// loop over all rules
	for (PatternCount rule: basicRules) {
		SEPARATOR_CHECK;
		stream << rule;
	}
	// we may have zero basic rules...
	for (PatternCompare rule: compareRules) {
		SEPARATOR_CHECK;
		stream << rule;
	}
	// we may have zero basic rules AND zero compare rules (!)...
	for (MultiRule rule: multiRules) {
		SEPARATOR_CHECK;
		stream << rule;
	}
	
	if (totalRules >= 2 || op == NONE) {
		stream << ")";
	}
};
