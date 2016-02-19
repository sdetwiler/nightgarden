//
//  rule.h
//  parser
//
//  Created by Stephen Detwiler on 2/18/16.
//  Copyright © 2016 Pirate Studios. All rights reserved.
//

#ifndef rule_h
#define rule_h


#include <string>
#include <map>


class Predicate;
class Result;
class SymbolList;
class Symbol;

////////////////////////////////////////////////////////////////////////////////
class Rule
{
public:
	Predicate* predicate;
	Result* result;
	
	Rule();
	
	~Rule();
	
	std::string toString() const;
	
	SymbolList* evaluate(Symbol* prev, Symbol* s, Symbol* next) const;
};



#endif /* rule_h */
