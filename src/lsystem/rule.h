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
	
	~Rule();
	
	std::string toString() const;
	
	SymbolList* evaluate(SymbolStack const& context, Symbol* s, Symbol* next) const;
};



#endif /* rule_h */
