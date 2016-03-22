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
	assign(rhs);
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

Symbol& Symbol::assign(Symbol const& rhs)
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

	return *this;
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
	
	s+= toString();
	
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

Symbol& Symbol::operator=(Symbol const& rhs)
{
	return assign(rhs);
}

float Symbol::applyGrowthFunction(float v) const
{
	// TODO: Implement more than linear growth functions.
	return linearGrowthFunction(v);
}

float Symbol::linearGrowthFunction(float v) const
{
	float max = 1.0;
	float pct = age/terminalAge;
	if(pct > max)
	{
		return v*max;
	}
	return v*pct;
}

