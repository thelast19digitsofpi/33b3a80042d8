// test.cpp
//
// I understand most people would probably use a testing library
// but once in my life I want to invent my own
// just as I've written my own linked list classes, etc

#include "Test.h"

#include <iostream>
#include "util.h"
#include "RuleNode.h"
#include "NumericCondition.h"


void Test::initialize() {
	successes = 0;
	total = 0;
};

Test::Test(const char* givenTitleCStr) {
	title = std::string(givenTitleCStr);
	initialize();
};

Test::Test(std::string givenTitle) {
	title = givenTitle;
	initialize();
};

void Test::printResults() {
	std::cout << "===== " << title << " =====" << std::endl;
	std::cout << BLUE << "Total: " << total << ' ';
	std::cout << GREEN << "Passed: " << successes << ' ';
	std::cout << RED << "Failed: " << (total - successes) << BLACK << std::endl;
	std::cout << "===== END TEST =====" << std::endl << std::endl;
};