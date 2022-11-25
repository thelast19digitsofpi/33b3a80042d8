// PatternCompare.cpp
//
// These classes are really unremarkable

#include "PatternCompare.h"


PatternCompare::PatternCompare(PatternFinder givenP1, PatternFinder givenP2, NumericOperator givenOp)
: p1(givenP1), p2(givenP2), op(givenOp) {};

PatternCompare::PatternCompare(std::string givenP1, std::string givenP2, NumericOperator givenOp)
: p1(givenP1), p2(givenP2), op(givenOp) {};

bool PatternCompare::evaluate(std::string givenString) {
	int n1 = p1.count(givenString);
	int n2 = p2.count(givenString);
	switch (op) {
		case LESS:
			return n1 < n2;
		case LESS_EQUAL:
			return n1 <= n2;
		case EQUAL:
			return n1 == n2;
		case GREATER_EQUAL:
			return n1 >= n2;
		case GREATER:
			return n1 > n2;
		case PARITY:
			// I don't think this will ever be used, but...
			// adding 2 numbers of the same parity gives an even number
			return (n1 + n2) % 2 == 0;
		default:
			return false;
	}
};

void PatternCompare::outputToStream(std::ostream& stream) const {
	switch (op) {
		case LESS:
			stream << "fewer";
			break;
		case LESS_EQUAL:
			stream << "at most as many";
			break;
		case EQUAL:
			stream << "the same number";
			break;
		case GREATER_EQUAL:
			stream << "at least as many";
			break;
		case GREATER:
			stream << "more";
			break;
		case PARITY:
			stream << "an even total";
			break;
	}
	
	stream << " of " << p1 << ' ';
	if (op == PARITY) {
		stream << "and";
	} else if (op == GREATER || op == LESS) {
		stream << "than";
	} else {
		stream << "as";
	}
	
	stream << ' ' << p2;
};
