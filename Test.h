// test.h
//
// needs a header file as much as anything else

#ifndef TEST_H_EXISTS
#define TEST_H_EXISTS

#include "util.h"
#include <iostream>
#include <string>

class Test {
	protected:
		void initialize();
		int successes;
		int total;
		std::string title;
	public:
		Test(const char*);
		Test(std::string);
		
		// Generically can assert equality of ints, bools, chars, even std::strings...
		template <typename T>
		void assertEqual(T actual, T expected, const char* name) {
			total++;
			if (actual == expected) {
				successes++;
			} else {
				std::cerr << RED << "Test Failed: " << name << std::endl;
				std::cerr << "Expected " << expected << ", found " << actual << BLACK << std::endl;
			}
		};
		
		void printResults();
};

#endif