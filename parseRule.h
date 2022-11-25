// parseRule.h
//
// There's seriously no point in making this a class.

#ifndef PARSERULE_H_EXISTS
#define PARSERULE_H_EXISTS

#include "MultiRule.h"
#include "ParseError.h"

#include <string>

// returns the number of rules or something? not sure...
void parseRule(std::string, MultiRule&);
void parseRule(const char*, MultiRule&);

#endif
