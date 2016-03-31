//
//  rule.cpp
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
}

Rule::~Rule()
{
	if(predicate)
	{
		delete predicate;
	}
}

string Rule::toString() const
{
	string s;
	
	s+= predicate->toString();
	s+= " -> ";
	s+= result.toString();
	
	return s;
}


bool Rule::evaluate(SymbolStack const& context, Symbol* currSymbol, Symbol* next, SymbolList& res) const
{
//	cout << "Rule::evaluate" << endl
//	<< "rule: " << this->toString() << endl
//	<< "smbl: " << s->toString() << endl;
	
	VariableMap* scope = predicate->createScope(context, currSymbol, next);
	
	// For every symbol in the rule's result, evaluate any expressions using the computed scope and then append to res.
 	for(SymbolList::const_iterator i = result.symbolList.begin(); i!=result.symbolList.end(); ++i)
	{
		Symbol s = *i;
		if(s.expressions)
		{
			for(ExpressionVec::iterator j = s.expressions->expressions.begin(); j!=s.expressions->expressions.end(); ++j)
			{
				Expression* e = *j;
				Variable v;
				
				if(e->eval(scope, &v))
				{
					if(v.type == Variable::Float)
					{
						e->value = to_string(v.value);
					}
					else if(v.type == Variable::String)
					{
						e->value = v.stringValue;
					}
				}
				else
				{
					delete scope;
					return false;
				}
			}
		}
		
		res.append(s);
	}
	
	delete scope;
	
//	cout << "rslt: " << res->toString() << endl << endl;
	return true;
}





ReferenceRule::ReferenceRule()
{
}

ReferenceRule::~ReferenceRule()
{
}

string ReferenceRule::toString() const
{
	return "ReferenceRule";
}

bool ReferenceRule::getReferencedSymbols(Symbol const* s, SymbolList& res) const
{
	if(s->expressions && s->expressions->expressions.size())
	{
		string filename = s->expressions->expressions[0]->value;
		SymbolListVec const* slv = LSystem::getCompiledStates(filename.c_str());
		if(slv)
		{
			float pct = s->age / s->terminalAge;
			if(pct > 1)
			{
				pct = 1;
			}
			else if(pct < 0)
			{
				pct = 0;
			}
			size_t idx = ((slv->size()-1) * pct);
			SymbolList const* src = &(*slv)[idx];
			res.append(*src);
			return true;
		}
	}
	
	return false;
}

bool ReferenceRule::evaluate(SymbolStack const& context, Symbol* s, Symbol* next, SymbolList& res) const
{
	return getReferencedSymbols(s, res);
}


