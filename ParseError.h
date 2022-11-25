// ParseError.h
//
// I remember back when I used to throw std::string's directly
// This is basically a named wrapper around std::string

#ifndef PARSEERROR_H_EXISTS
#define PARSEERROR_H_EXISTS

#include <iostream>

class ParseError {
public:
	std::string message;
	ParseError(std::string);
	ParseError(const char*);
};

std::ostream& operator<<(std::ostream&, ParseError&);

#endif