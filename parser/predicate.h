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

class Symbol;
class Expression;
class VariableMap;

////////////////////////////////////////////////////////////////////////////////
class Predicate
{
public:
	Symbol* symbol;
	Symbol* prev;	// Previous symbol for context-sensitive evaluation.
	Symbol* next;	// Next symbol for context-sensitive evaluation.
	Expression* condition;
	Predicate();
	~Predicate();
	
	std::string toString() const;
	bool doesMatch(Symbol const* prevSymbol, Symbol const* currSymbol, Symbol const* nextSymbol);
	VariableMap* createScope(Symbol const* prevSymbol, Symbol const* currSymbol, Symbol const* nextSymbol) const;

};


#endif /* predicate_h */
