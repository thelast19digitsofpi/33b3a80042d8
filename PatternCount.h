// PatternCount.h
//
// Counts the number of matches of a PatternFinder and evaluates to TRUE iff it fits a NumericCondition.


#ifndef PATTERNCOUNT_H_EXISTS
#define PATTERNCOUNT_H_EXISTS

#include "RuleNode.h"
#include "PatternFinder.h"
#include "NumericCondition.h"

#include <string>

// 
class PatternCount: public RuleNode {
	protected:
		PatternFinder pattern;
		NumericCondition condition;
		
		void outputToStream(std::ostream&) const;
	public:
		PatternCount(const PatternFinder&, const NumericCondition&);
		PatternCount(std::string, int, NumericOperator);
		PatternCount(char, int, NumericOperator);
		
		bool evaluate(std::string);
};

#endif