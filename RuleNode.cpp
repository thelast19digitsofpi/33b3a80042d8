// RuleNode.cpp
//
// all it really does is provide one common method...

#include "RuleNode.h"
#include <string>
#include <sstream>

// Simple method for converting to a string if the operator is defined
std::string RuleNode::toString() const {
	std::stringstream ss;
	outputToStream(ss);
	return ss.str();
};

// make it actually work
std::ostream& operator<<(std::ostream& outstream, const RuleNode& node) {
	node.outputToStream(outstream);
	return outstream;
};
