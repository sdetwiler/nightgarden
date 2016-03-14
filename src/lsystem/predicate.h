//
//  predicate.h
//  parser
//
//  Created by Stephen Detwiler on 2/20/16.
//  Copyright © 2016 Pirate Studios. All rights reserved.
//

#ifndef predicate_h
#define predicate_h

#include <string>
#include "types.h"
#include "expression.h"

//class Symbol;
//class Expression;
//class VariableMap;
class LSystem;

////////////////////////////////////////////////////////////////////////////////
class Predicate
{
public:
	Symbol* symbol;
	Symbol* prev;	// Previous symbol for context-sensitive evaluation.
	Symbol* next;	// Next symbol for context-sensitive evaluation.
	Expression* condition; // Condition that must be true for predicate to match symbol sequence.
	LSystem const* lsystem;
	
	Predicate(LSystem const* lsystem);
	~Predicate();
	
	std::string toString() const;
	bool doesMatch(SymbolStack const& context, Symbol const* currSymbol, Symbol const* nextSymbol);
	VariableMap* createScope(SymbolStack const& context, Symbol const* currSymbol, Symbol const* nextSymbol) const;
};


#endif /* predicate_h */
