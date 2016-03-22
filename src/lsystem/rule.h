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
	Result result;
	
	Rule();
	
	virtual ~Rule();
	
	virtual std::string toString() const;
	
	// Evaluates the rule and appends the results to the passed SymbolList result.
	virtual bool evaluate(SymbolStack const& context, Symbol* currSymbol, Symbol* next, SymbolList& result) const;
};



////////////////////////////////////////////////////////////////////////////////
class ReferenceRule : public Rule
{
public:
	ReferenceRule();
	
	virtual ~ReferenceRule();
	
	virtual std::string toString() const;
	
	// Evaluates the rule and appends the results to the passed SymbolList result.
	virtual bool evaluate(SymbolStack const& context, Symbol* currSymbol, Symbol* next, SymbolList& result) const;
	bool getReferencedSymbols(Symbol const* currSymbol, SymbolList& result) const;
};



#endif /* rule_h */
