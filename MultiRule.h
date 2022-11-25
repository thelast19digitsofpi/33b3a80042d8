// MultiRule.h
//
// Conjunction of several rules

#ifndef MULTIRULE_H_EXISTS
#define MULTIRULE_H_EXISTS

#include "PatternCount.h"
#include "PatternCompare.h"

#include <ostream>
// This one might have to be one you build as you go
#include <vector>

// MultiRules need to know how to aggregate
// almost called them nor, xor, or, and... but "xor" doesn't work that way with 3+
// actually, ONLY_ONE is still unused...
enum BooleanOperator {
	BOOLEAN_UNDEFINED = -1,
	NONE, ONLY_ONE, SOME, ALL
};

// This is a nice way to combine several types of rules...
class MultiRule: public RuleNode {
	protected:
		std::vector<PatternCount>   basicRules;
		std::vector<PatternCompare> compareRules;
		std::vector<MultiRule>      multiRules;
		BooleanOperator op;
		
		int totalRules = 0;
	public:
		// You have to build it piece by piece
		MultiRule();
		MultiRule(BooleanOperator);
		~MultiRule();
		
		// shortcuts
		// note: these will force the "SOME" quantifier...
		MultiRule(PatternCount&&);
		MultiRule(PatternCompare&&);
		
		BooleanOperator getOp();
		void setOp(BooleanOperator);
		
		// I feel so clever here
		void addRule(PatternCount);
		void addRule(PatternCompare);
		// so theoretically any possible rule can be done with enough multi rule nesting
		void addRule(MultiRule);
		// one evaluate function to, uh, implement in the cpp file
		bool evaluate(std::string);
		
		int getTotalRules() const;
		
		void outputToStream(std::ostream&) const final;
};

#endif