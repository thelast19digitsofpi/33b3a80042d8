// PatternFinder.h
//
// Checks the number of instances of a specific pattern.
// Note that there is no evaluate(), only count().
// Supported patterns: letters AEIOU are literal, 12345 are distinct wildcards (e.g. 121 matches AEA but not III), ? is a general one-letter wildcard (e.g. 1? matches both AA and AE).
// Boolean parameters (must start with, must end with) sort of wrap around the pattern string...
// The only variable-length option is *, which matches any one or more letters.


#ifndef PATTERNFINDER_H_EXISTS
#define PATTERNFINDER_H_EXISTS

#include <string>

#include "RuleNode.h"

class PatternFinder: public RuleNode {
	protected:
		bool mustStart;
		std::string pattern;
		bool mustEnd;
	public:
		PatternFinder(char);
		PatternFinder(std::string);
		PatternFinder(bool, std::string, bool);
		PatternFinder(bool, const char*, bool);
		
		static void validatePattern(std::string);
		
		int count(std::string) const;
		int count(const char*) const;
		
		void outputToStream(std::ostream&) const;
};


#endif
