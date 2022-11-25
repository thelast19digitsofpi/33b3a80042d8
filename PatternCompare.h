// PatternCompare.h
//
// Another type of rule: more instances of one pattern than another pattern

#ifndef PATTERNCOMPARE_H_EXISTS
#define PATTERNCOMPARE_H_EXISTS

#include "PatternFinder.h"
// NumericCondition contains the NumericOperator enum
#include "NumericCondition.h"

class PatternCompare: public RuleNode {
	protected:
		PatternFinder p1;
		PatternFinder p2;
		NumericOperator op;
	public:
		PatternCompare(PatternFinder, PatternFinder, NumericOperator);
		PatternCompare(std::string, std::string, NumericOperator);
		
		bool evaluate(std::string);
		void outputToStream(std::ostream&) const;
};
#endif