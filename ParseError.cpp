// ParseError.cpp
//
// Maybe it's fine to put this in the header, but where do you draw the line?

#include "ParseError.h"

ParseError::ParseError(std::string givenMessage)
: message(givenMessage) {};

ParseError::ParseError(const char* givenMessage)
: message(givenMessage) {};

std::ostream& operator<<(std::ostream& stream, ParseError& error) {
	stream << "Sieve Error: " << error.message;
	return stream;
};
