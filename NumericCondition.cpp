// NumericCondition.cpp
//
// This expresses a numeric condition, like "at least 4" of something.

#include "NumericCondition.h"


NumericCondition::NumericCondition(int givenNumber, NumericOperator givenOp)
: number(givenNumber), operation(givenOp) {};

bool NumericCondition::evaluate(int input) const {
	switch (operation) {
		case LESS:          return input < number;
		case LESS_EQUAL:    return input <= number;
		case GREATER:       return input > number;
		case GREATER_EQUAL: return input >= number;
		case EQUAL:         return input == number;
		case PARITY:
			// parity(x, any odd number) is a special way of saying "is x odd", etc
			return (input & 1) == (number & 1);
		default:
			return false;
	}
};

void NumericCondition::outputToStream(std::ostream& stream) const {
	// puts this to words
	switch (operation) {
		case LESS:
			if (number == 1) {
				stream << "none";
			} else {
				// "less than 4" means "at most 3"
				stream << "at most " << (number - 1);
			}
			break;
		case LESS_EQUAL:
			// maybe I should ditch LESS and GREATER entirely
			// eh, no, I need them for two-item comparisons
			if (number == 0) {
				stream << "none";
			} else {
				stream << "at most " << number;
			}
			break;
		case GREATER:
			if (number == 0) {
				stream << "some";
			} else {
				stream << "at least " << (number + 1);
			}
			break;
		case GREATER_EQUAL:
			if (number == 0) {
				// this is kind of weird
				stream << "any number [wait, what?!]";
			} else if (number == 1) {
				stream << "some";
			} else {
				stream << "at least " << number;
			}
			break;
		case EQUAL:
			if (number == 0) {
				stream << "none";
			} else {
				stream << "exactly " << number;
			}
			break;
		case PARITY:
			stream << "an ";
			if ((number & 1) == 1) {
				stream << "odd";
			} else {
				stream << "even";
			}
			stream << " number";
			break;
		default:
			stream << "Error: Invalid numeric operator, code: " << (int)operation << std::endl;
	}
};