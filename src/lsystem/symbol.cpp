//
//  symbol.cpp
//  nightgarden
//
//  Created by Stephen Detwiler on 3/21/16.
//
//

#include "symbol.h"
#include "types.h"

using namespace std;

Symbol::Symbol()
{
	isOperator = false;
	variables = nullptr;
	expressions = nullptr;
	age = 0;
	terminalAge = 1;
	isTerminal = false;
}

Symbol::Symbol(Symbol const& rhs)
{
	isOperator = rhs.isOperator;
	value = rhs.value;
	terminalAge = rhs.terminalAge;
	age = rhs.age;
	isTerminal = rhs.isTerminal;
	
	if(rhs.variables)
	{
		variables = new VariableList(*(rhs.variables));
	}
	else
	{
		variables = nullptr;
	}
	if(rhs.expressions)
	{
		expressions = new ExpressionList(*(rhs.expressions));
	}
	else
	{
		expressions = nullptr;
	}
	
}

Symbol::~Symbol()
{
	if(variables)
	{
		delete variables;
	}
	if(expressions)
	{
		delete expressions;
	}
}

std::string Symbol::toString() const
{
	string s;
	
	s+= value;
	if(variables && variables->variables.size())
	{
		s+= "(";
		
		s+= variables->toString();
		
		s+= ")";
	}
	
	if(expressions && expressions->expressions.size())
	{
		
		s+= "(";
		s+= expressions->toString();
		s+= ")";
	}
	
	return s;
}

std::string Symbol::toTimedString() const
{
	string s;
	s+= "(";
	
	s+= value;
	if(variables && variables->variables.size())
	{
		s+= "(";
		
		s+= variables->toString();
		
		s+= ")";
	}
	
	if(expressions && expressions->expressions.size())
	{
		
		s+= "(";
		s+= expressions->toString();
		s+= ")";
	}
	
	s+=", ";
	s+=to_string(age);
	s+=", ";
	s+=to_string(terminalAge);
	s+=")";
	
	return s;
}

bool Symbol::operator==(Symbol const& rhs) const
{
	return value == rhs.value;
}

bool Symbol::operator!=(Symbol const& rhs) const
{
	return value != rhs.value;
}

// Use the symbol's current age to scale the value v by a growth function.
// Used to animate the current symbol.
float Symbol::applyGrowthFunction(float v)
{
	// TODO: Implement more than linear growth functions.
	return linearGrowthFunction(v);
}

float Symbol::linearGrowthFunction(float v)
{
	float max = 1.0;
	float pct = age/terminalAge;
	if(pct > max)
	{
		return v*max;
	}
	return v*pct;
}
