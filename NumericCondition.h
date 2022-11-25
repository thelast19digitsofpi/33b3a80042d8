// NumericCondition.h
//
// Supported: equals, less, greater, parity
// (parity means "same parity as the given int")

#ifndef NUMERICCONDITION_H_EXISTS
#define NUMERICCONDITION_H_EXISTS


#include "RuleNode.h"

enum NumericOperator: char {
	// like interval notation: [2, 5)
	LESS = ')',
	LESS_EQUAL = ']',
	EQUAL = '=',
	GREATER_EQUAL = '[',
	GREATER = '(',
	PARITY = '%'
};


class NumericCondition: public RuleNode {
protected:
	int number;
	NumericOperator operation;
	void outputToStream(std::ostream&) const;
public:
	NumericCondition() = delete;
	NumericCondition(int, NumericOperator);
	
	bool evaluate(int) const;
};

#endif
