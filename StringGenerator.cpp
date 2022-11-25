// StringGenerator.cpp
//
// this isn't too complicated...

#include "StringGenerator.h"

StringGenerator::StringGenerator(int givenMaxLength)
: maxLength(givenMaxLength), curLength(1), index(0) {};

std::string StringGenerator::next() {
	if (isDone()) {
		// return an empty string
		return std::string();
	}
	
	char word[maxLength + 1];
	
	// 6 just to avoid hassles with the null terminated string
	char alphabet[6] = "AEIOU";
	
	bool doneWithThisLength = true;
	
	// index encodes which word we are on...
	// the base 5 digits become letters
	int remainder = index;
	for (int i = 0; i < curLength; i++) {
		// unless we are at 44...4 (base 5), stay on this length
		if (remainder % 5 != 4) {
			doneWithThisLength = false;
		}
		// in base 5, this is taking the 1's digit and then forgetting that digit
		word[i] = alphabet[remainder % 5];
		remainder /= 5; // integer division is powerful, I use it frequently
	}
	// need the null termination!
	word[curLength] = '\0';
	
	// go to the next word
	if (doneWithThisLength) {
		index = 0;
		curLength++;
	} else {
		index++;
	}
	
	return std::string(word);
};

bool StringGenerator::isDone() {
	return curLength > maxLength;
};