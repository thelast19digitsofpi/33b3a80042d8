// util.cpp
//
// here we go

#include "util.h"

#include <random>
#include <chrono>
#include <climits>

bool isValidLetter(char c) {
	return c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U';
};

std::string uppercase(std::string s) {
	// make a copy of the string, and convert it to uppercase
	std::string result(s);
	int length = s.length();
	for (int i = 0; i < length; i++) {
		result[i] = ::toupper(result[i]);
	}
	return result;
};

std::string uppercase(const char* s) {
	return uppercase(std::string(s));
};

static std::mt19937 generator;

void setupRandom() {
	// thanks https://www.cplusplus.com/reference/random/mersenne_twister_engine/operator()/
	generator = std::mt19937(std::chrono::system_clock::now().time_since_epoch().count());
};

int getRandom() {
	// shift to clear sign bit, because c++'s mod function is illogical wrt negatives
	return (int)(generator() >> 1);
};

int getRandom(int maxPlusOne) {
	return getRandom() % maxPlusOne;
};