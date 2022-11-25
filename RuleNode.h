// RuleNode.h
//
// Base class for parsing user-entered rules from the console.

#ifndef RULENODE_H_EXISTS
#define RULENODE_H_EXISTS

#include <iostream>
#include <string>

class RuleNode {
protected:
	virtual void outputToStream(std::ostream&) const = 0;
public:
	RuleNode() = default;
	virtual ~RuleNode() = default;
	std::string toString() const;
	friend std::ostream& operator<<(std::ostream&, const RuleNode&);
};

#endif
