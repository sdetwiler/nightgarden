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
					if(v.type == Variable::Float)
					{
						e->value = to_string(v.value);
					}
					else if(v.type == Variable::String)
					{
						e->value = v.stringValue;
					}
				}
			}
		}
	}
	
	delete scope;
	
//	cout << "rslt: " << res->toString() << endl << endl;
	return res;
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


SymbolList* ReferenceRule::evaluate(SymbolStack const& context, Symbol* s, Symbol* next) const
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
			if(pct < 0)
			{
				pct = 0;
			}
			size_t idx = ((slv->size()-1) * pct);
			// Another copy? FIXME
			return new SymbolList(*(*slv)[idx]);
		}
	}
	
	return nullptr;
}


