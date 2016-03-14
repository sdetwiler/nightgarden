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

using namespace std;

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

string Rule::toString() const
{
	string s;
	
	s+= predicate->toString();
	s+= " -> ";
	s+= result->toString();
	
	return s;
}


SymbolList* Rule::evaluate(SymbolStack const& context, Symbol* s, Symbol* next) const
{
//	cout << "Rule::evaluate" << endl
//	<< "rule: " << this->toString() << endl
//	<< "smbl: " << s->toString() << endl;
	
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
					e->value = to_string(v.value);
				}
			}
		}
	}
	
//	cout << "rslt: " << res->toString() << endl << endl;
	return res;
}
