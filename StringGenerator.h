// StringGenerator.h
//
// Used to brute force all of the strings possible in a game.
// next() gets the next string and isDone() checks if there are any left.
// This way, looping through every string (e.g. to check for counterexamples)
// is as simple as:
/*
while (!generator.isDone()) {
	std::string word = generator.next();
	// do stuff
}
*/

#ifndef STRINGGENERATOR_H_EXISTS
#define STRINGGENERATOR_H_EXISTS

#include <string>

class StringGenerator {
	protected:
		const int maxLength;
		int curLength = 1;
		int index = 0;
	public:
		StringGenerator(int);
		std::string next();
		bool isDone();
};
#endif
