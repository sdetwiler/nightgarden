//
//  symbol.h
//  nightgarden
//
//  Created by Stephen Detwiler on 3/21/16.
//
//

#ifndef symbol_h
#define symbol_h

#include <string>

class VariableList;
class ExpressionList;

////////////////////////////////////////////////////////////////////////////////
class Symbol
{
public:
	std::string value;
	bool isOperator;
	
	bool isTerminal;
	float terminalAge;
	float age;
	
	VariableList* variables; // (parameters)
	ExpressionList* expressions;
	
	Symbol();
	Symbol(Symbol const& rhs);
	~Symbol();
	std::string toString() const;
	std::string toTimedString() const;

	bool operator==(Symbol const& rhs) const;
	bool operator!=(Symbol const& rhs) const;
	
	// Use the symbol's current age to scale the value v by a growth function.
	// Used to animate the current symbol.
	float applyGrowthFunction(float v);
	float linearGrowthFunction(float v);
};


#endif /* symbol_h */
