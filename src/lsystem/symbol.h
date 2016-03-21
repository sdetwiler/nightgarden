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
	std::string value;				// The symbol value.
	bool isOperator;				// True if the symbol is an lsystem turtle operator.
	
	bool isTerminal;				// Has the symbol reached it's terminal age.
	float terminalAge;				// The symbol's terminal age.
	float age;						// The symbol's current age.
	
	VariableList* variables;		// parameter variable names in Predicate.
	ExpressionList* expressions;	// Parameter variable values in Result.
	
	Symbol();
	Symbol(Symbol const& rhs);
	
	~Symbol();
	
	// Returns lsystem string encoding of symbol.
	std::string toString() const;
	
	// Returns timed lsystem string encoding of symbol.
	std::string toTimedString() const;

	bool operator==(Symbol const& rhs) const;
	bool operator!=(Symbol const& rhs) const;
	
	// Use the symbol's current age to scale the value v by a growth function.
	// Used to animate the current symbol.
	float applyGrowthFunction(float v);
	float linearGrowthFunction(float v);
};


#endif /* symbol_h */
