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

void LSystem::parse(char const* input)
{
	
	YY_BUFFER_STATE buf;
	
	buf = yy_scan_string(input);
	
	yyparse();
	
	yy_delete_buffer(buf);
}


void LSystem::setAxiom(SymbolList *axiom)
{
	if(mAxiom)
	{
		delete mAxiom;
	}
	
	std::cout << "setAxiom" << std::endl;
	mAxiom = axiom;
	if(mState)
	{
		delete mState;
	}
	mState = new SymbolList(*mAxiom);
}


void LSystem::addRule(Rule* rule)
{
	std::cout << "addRule" << std::endl;
	mRules.push_back(rule);
}

void LSystem::dumpRules()
{
	for(RuleVec::iterator i = mRules.begin(); i!=mRules.end(); ++i)
	{
		std::cout << (*i)->toString() << std::endl;
	}
}

void LSystem::clear()
{
	for(RuleVec::iterator i = mRules.begin(); i!=mRules.end(); ++i)
	{
		delete (*i);
	}
	
	mRules.clear();
	
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

Rule const* LSystem::getRuleForSymbol(Symbol const& s)
{
	for(RuleVec::iterator i = mRules.begin(); i!=mRules.end(); ++i)
	{
		Rule const* curr = (*i);
		if(*(curr->predicate->symbol) == s)
		{
			return curr;
		}
	}
	
	return nullptr;
}


void LSystem::step()
{
	SymbolList* output = new SymbolList;
	
	for(SymbolVec::const_iterator i = mState->symbols.begin(); i!=mState->symbols.end(); ++i)
	{
		Symbol* s = (*i);
		Rule const* rule = getRuleForSymbol(*s); // TODO: Context aware evaluation...
		if(rule)
		{
			*(output)+= *(rule->result->symbolList);
		}
	}
	
	if(mState)
	{
		delete mState;
	}
	
	mState = output;
	
}
