//
//  lsystem.cpp
//  parser
//
//  Created by Stephen Detwiler on 2/15/16.
//  Copyright © 2016 Pirate Studios. All rights reserved.
//

#include "lsystem.h"
#include <iostream>

#include "y.tab.h"

// Parser forward declarations.
#ifndef FLEXINT_H

typedef struct yy_buffer_state *YY_BUFFER_STATE;
YY_BUFFER_STATE  yy_scan_string(const char *s);

int yyparse();
void yy_delete_buffer(YY_BUFFER_STATE buf);

#endif



LSystem::LSystem()
{
	mAxiom = nullptr;
	mState = nullptr;
}

LSystem::~LSystem()
{
	clear();
}

LSystem& LSystem::getInstance()
{
	static LSystem* singleton = nullptr;
	if(singleton == nullptr)
	{
		singleton = new LSystem;
	}
	
	return *singleton;
}

bool LSystem::parse(char const* input)
{
	
	YY_BUFFER_STATE buf;
	
	buf = yy_scan_string(input);
	
	int ret = yyparse();
	
	yy_delete_buffer(buf);

	if(ret)
	{
//		std::cout << "yyparse returns " << ret << std::endl;
		return false;
	}

	return true;
}


void LSystem::setAxiom(SymbolList *axiom)
{
	if(mAxiom)
	{
		delete mAxiom;
	}
	
	mAxiom = axiom;
	if(mState)
	{
		delete mState;
	}
	mState = new SymbolList(*mAxiom);
}


void LSystem::addRule(Rule* rule)
{
	mRules.push_back(rule);
}

void LSystem::dumpRules()
{
	for(RuleVec::iterator i = mRules.begin(); i!=mRules.end(); ++i)
	{
		std::cout << (*i)->toString() << std::endl;
	}
}

void LSystem::addVariable(Variable* variable)
{
	mVariables[variable->name] = variable;
}

void LSystem::dumpVariables()
{
	for(StringVariableMap::iterator i = mVariables.begin(); i!=mVariables.end(); ++i)
	{
		std::cout << i->second->toString() << std::endl;
	}
}


void LSystem::clear()
{
	for(RuleVec::iterator i = mRules.begin(); i!=mRules.end(); ++i)
	{
		delete (*i);
	}
	mRules.clear();

	for(StringVariableMap::iterator i = mVariables.begin(); i!=mVariables.end(); ++i)
	{
		delete i->second;
	}
	mVariables.clear();

	if(mAxiom)
	{
		delete mAxiom;
		mAxiom = nullptr;
	}
	
	if(mState)
	{
		delete mState;
		mState = nullptr;
	}
}



SymbolList const* LSystem::getState()
{
	return mState;
}

Rule const* LSystem::getRuleForSymbol(Symbol const* prev, Symbol const* symbol, Symbol const* next)
{
	for(RuleVec::iterator i = mRules.begin(); i!=mRules.end(); ++i)
	{
		Rule const* curr = (*i);
		if(curr->predicate->doesMatch(prev, symbol, next))
		{
			return curr;
		}
	}
	
	return nullptr;
}


void LSystem::step()
{
	SymbolList* output = new SymbolList;
	
	Symbol* prev = nullptr;
	Symbol* next = nullptr;
	for(SymbolVec::const_iterator i = mState->symbols.begin(); i!=mState->symbols.end(); ++i)
	{
		Symbol* symbol = (*i);

		// Peek at the next symbol.
		SymbolVec::const_iterator peek = i+1;
		if(peek !=mState->symbols.end())
		{
			next = *peek;
		}
		else
		{
			next = nullptr;
		}
		
		Rule const* rule = getRuleForSymbol(prev, symbol, next);
		if(rule)
		{
			*(output)+= *(rule->result->symbolList);
		}
		// If no rule is found that matches the current symbol, append the original symbol to the output.
		else
		{
			*(output)+= *symbol;
		}
		
		prev = symbol;
	}
	
	if(mState)
	{
		delete mState;
	}
	
	mState = output;
	
}
