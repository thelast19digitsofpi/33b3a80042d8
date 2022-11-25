// util.h
//
// almost every project of mine has a utility header or file these days

#ifndef UTIL_H_EXISTS
#define UTIL_H_EXISTS

#include <string>

#define RED "\e[31;1m"
#define GREEN "\e[32;1m"
#define BLUE "\e[34;1m"
#define YELLOW "\e[33;1m"
#define BLACK "\e[30;0m"

bool isValidLetter(char);

// the c++ standard library really should do this...
std::string uppercase(std::string);
std::string uppercase(const char*);

void setupRandom();
int getRandom();
int getRandom(int);

#endif
