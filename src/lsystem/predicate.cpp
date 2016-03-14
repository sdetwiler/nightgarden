//
//  predicate.cpp
//  parser
//
//  Created by Stephen Detwiler on 2/20/16.
//  Copyright © 2016 Pirate Studios. All rights reserved.
//

#include "predicate.h"
#include "types.h"
#include "lsystem.h"

using namespace std;

Predicate::Predicate(LSystem const* system)
{
	symbol = nullptr;
	prev = nullptr;
	next = nullptr;
	condition = nullptr;
	lsystem = system;
}

Predicate::~Predicate()
{
	if(symbol)
	{
		delete symbol;
	}
	
	if(prev)
	{
		delete prev;
	}
	
	if(next)
	{
		delete next;
	}
	
	if(condition)
	{
		delete condition;
	}
}

std::string Predicate::toString() const
{
	string s;
	
	if(prev)
	{
		s+= prev->toString();
		s+= "<";
	}
	
	if(symbol)
	{
		s+=symbol->toString();
	}
	
	if(next)
	{
		s+= ">";
		s+= next->toString();
	}
	
	if(condition)
	{
		s+= " : ";
		s+= condition->toString();
	}
	
	return s;
}

bool Predicate::doesMatch(SymbolStack const& context, Symbol const* currSymbol, Symbol const* nextSymbol)
{
	// TODO: Multisymbol predicate previous components that can evaluate against the computed context.
	if(prev)
	{
		if(context.size())
		{
			// The immediate predecessor to the current symbol is at the front of the context.
			if(*prev != *context.top())
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	
	if(next)
	{
		if(nextSymbol)
		{
			if(*next != *nextSymbol)
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	
	if(*symbol != *currSymbol)
	{
		return false;
	}
	
	
	if(condition)
	{
		
		VariableMap* scope = createScope(context, currSymbol, nextSymbol);
		Variable ret;
		if(condition->eval(scope, &ret))
		{
			delete scope;
			if(ret.value>0)
			{
				return true;
			}
			return false;
		}
		else
		{
			cout << "condition expression eval failed for " << condition->toString() << endl << "scope: " << scope->toString() << endl;
			delete scope;
		}
		
	}
	
	return true;
}



VariableMap* Predicate::createScope(SymbolStack const& context, Symbol const* currSymbol, Symbol const* nextSymbol) const
{
	// The predicate defines variable names contained in the current scope.
	// The prev, curr and next symbols contains expressions that set the
	// variable values. Values are mapped in the order the variables are
	// defined from the predicate.

	VariableMap* scope = new VariableMap(lsystem->getGlobalVariables());
	
	// If the predicate's symbol has attached variables...
	size_t idx=0;
	if(prev && prev->variables)
	{
		// TODO multi symbol previous components...
		Symbol* prevSymbol = nullptr;
		if(context.size())
		{
			prevSymbol = context.top();
		}
		idx = scope->addToMap(prev->variables, prevSymbol, idx);
	}
	if(symbol && symbol->variables)
	{
		idx = scope->addToMap(symbol->variables, currSymbol, idx);
	}
	if(next && next->variables)
	{
		idx = scope->addToMap(next->variables, nextSymbol, idx);
	}

	return scope;
}

