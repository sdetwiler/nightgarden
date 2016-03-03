//
//  rule.cpp
//  parser
//
//  Created by Stephen Detwiler on 2/18/16.
//  Copyright © 2016 Pirate Studios. All rights reserved.
//

#include "rule.h"
#include "types.h"
#include "lsystem.h"
#include "predicate.h"

Rule::Rule()
{
	predicate = nullptr;
	result = nullptr;
}

Rule::~Rule()
{
	if(predicate)
	{
		delete predicate;
	}
	
	if(result)
	{
		delete result;
	}
}

std::string Rule::toString() const
{
	std::string s;
	
	s+= predicate->toString();
	s+= " -> ";
	s+= result->toString();
	
	return s;
}


SymbolList* Rule::evaluate(SymbolVec const& context, Symbol* s, Symbol* next) const
{
//	std::cout << "Rule::evaluate" << std::endl
//	<< "rule: " << this->toString() << std::endl
//	<< "smbl: " << s->toString() << std::endl;
	
	VariableMap* scope = predicate->createScope(context, s, next);
	
	SymbolList* res = new SymbolList(*(result->symbolList));
	
	// For every symbol in the symbol list, evaluate their expressions using the computed scope.
	for(SymbolVec::iterator i = res->symbols.begin(); i!=res->symbols.end(); ++i)
	{
		Symbol* s = *i;
		if(s->expressions)
		{
			for(ExpressionVec::iterator j = s->expressions->expressions.begin(); j!=s->expressions->expressions.end(); ++j)
			{
				Expression* e = *j;
				
				Variable v;

				if(e->eval(scope, &v))
				{
					e->value = std::to_string(v.value);
				}
			}
		}
	}
	
//	std::cout << "rslt: " << res->toString() << std::endl << std::endl;
	return res;
}
