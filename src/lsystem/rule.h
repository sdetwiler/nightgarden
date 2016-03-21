//
//  rule.h
//
//  Created by Stephen Detwiler on 2/18/16.
//  Copyright © 2016 Pirate Studios. All rights reserved.
//

#ifndef rule_h
#define rule_h


#include <string>
#include <map>

#include "types.h"
#include "expression.h"
#include "predicate.h"


////////////////////////////////////////////////////////////////////////////////
class Rule
{
public:
	Predicate* predicate;
	Result* result;
	
	Rule();
	
	virtual ~Rule();
	
	virtual std::string toString() const;
	
	virtual SymbolList* evaluate(SymbolStack const& context, Symbol* s, Symbol* next) const;
};



////////////////////////////////////////////////////////////////////////////////
class ReferenceRule : public Rule
{
public:
	ReferenceRule();
	
	virtual ~ReferenceRule();
	
	virtual std::string toString() const;
	
	virtual SymbolList* evaluate(SymbolStack const& context, Symbol* s, Symbol* next) const;
	SymbolList* getReferencedSymbols(Symbol* s) const;
};



#endif /* rule_h */
