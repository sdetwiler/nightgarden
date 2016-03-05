//
//  lsystem.cpp
//  parser
//
//  Created by Stephen Detwiler on 2/15/16.
//  Copyright © 2016 Pirate Studios. All rights reserved.
//

#include "lsystem.h"
#include "predicate.h"

#include <iostream>
#include <fstream>
#include <stack>

#include "rules_parser.tab.h"

// Parser forward declarations.
#ifndef FLEXINT_H

typedef struct yy_buffer_state *YY_BUFFER_STATE;

YY_BUFFER_STATE  rules__scan_string(const char *s);
void rules__delete_buffer(YY_BUFFER_STATE buf);

int rules_parse();

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

bool LSystem::load(char const* filename)
{
//	char const* filename = "/Users/steve/projects/nightgarden/data/test.ls";
	std::ifstream infile(filename);
	if(!infile.is_open())
	{
		std::cout << "Failed to open " << filename << std::endl;
		return -1;
	}
	std::string data;
	std::string line;
	while(std::getline(infile, line))
	{
		data+=line;
		data+="\n";
	}
	
	if(infile.bad())
	{
		std::cout << "Error while reading from " << filename << std::endl;
		return -1;
	}
	
	LSystem& lsystem = LSystem::getInstance();
	return lsystem.parse(data.c_str());
}

bool LSystem::parse(char const* input)
{
	
	YY_BUFFER_STATE buf;
	
	buf = rules__scan_string(input);
	
	int ret = rules_parse();
	
	rules__delete_buffer(buf);

	if(ret)
	{
//		std::cout << "yyparse returns " << ret << std::endl;
		return false;
	}

	return true;
}


void LSystem::setAxiom(Result* axiom)
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
	mState = new SymbolList(*(mAxiom->symbolList));
}


void LSystem::addRule(Rule* rule)
{
	mRules.push_back(rule);
}

void LSystem::dumpRules()
{
	std::cout << "LSystem::dumpRules" << std::endl;
	for(RuleVec::iterator i = mRules.begin(); i!=mRules.end(); ++i)
	{
		std::cout << (*i)->toString() << std::endl;
	}
	std::cout << std::endl;
	
}

void LSystem::addVariable(Variable* variable)
{
	mVariables.variables[variable->name] = variable;
}

void LSystem::dumpVariables()
{
	std::cout << "LSystem::dumpVariables" << std::endl;
	std::cout << mVariables.toString() << std::endl;
	std::cout << std::endl;
}


void LSystem::clear()
{
	for(RuleVec::iterator i = mRules.begin(); i!=mRules.end(); ++i)
	{
		delete (*i);
	}
	mRules.clear();

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

float LSystem::getGlobalVariable(char const* name, float def)
{
	float ret = def;
	StringVariableMap::const_iterator i;
	i = mVariables.variables.find(std::string(name));
	if(i!=mVariables.variables.end())
	{
		ret = i->second->value;
	}
	return ret;
}


VariableMap const& LSystem::getGlobalVariables()
{
	return mVariables;
}


SymbolList const* LSystem::getState()
{
	return mState;
}


Rule const* LSystem::getRuleForSymbol(SymbolVec const& context, Symbol const* symbol, Symbol const* next)
{
	for(RuleVec::iterator i = mRules.begin(); i!=mRules.end(); ++i)
	{
		Rule const* curr = (*i);
		
		
		if(curr->predicate->doesMatch(context, symbol, next))
		{
			return curr;
		}
	}
	
	return nullptr;
}

void LSystem::step()
{
	if(!mState)
	{
		return;
	}
	SymbolList* output = new SymbolList;

	//typedef std::stack<Symbol*> SymbolStack;
	// Not as efficient as using a stack<> but ...
	SymbolVec context;
	Symbol root;
	context.push_back(&root);
	SymbolVec branches;
	for(SymbolVec::const_iterator i = mState->symbols.begin(); i!=mState->symbols.end(); ++i)
	{
		Symbol* symbol = (*i);

		// If a push branch symbol...
		if(symbol->value == "[")
		{
			// Add the symbol to the output.
			*(output)+= *symbol;

			// Store the current head of the context in the branches vector.
//			if(context.size())
//			{
				branches.insert(branches.begin(), context.front());
//			}
		}
		
		// If a pop branch symbol...
		else if(symbol->value == "]")
		{
			// Add the symbol to the output.
			*(output)+= *symbol;

			// Get and pop location of the last branch and rewind the context back to that point.
			if(branches.size() == 0)
			{
				std::cout << "Mismatch square brackets";
				context.clear();
			}
			else
			{
				Symbol* b = branches.front();
				branches.erase(branches.begin());
				while(context.size())
				{
					if(b == context.front())
					{
						break;
					}
					context.erase(context.begin());
				}
			}
		}
		
		else if(symbol->isOperator)
		{
			*(output)+= *symbol;
			continue;
		}
		

		// HACK: Only one lookahead symbol currently provided.
		// Peek at the next symbol.
		Symbol* next = nullptr;
		SymbolVec::const_iterator peek = i+1;
		while(peek != mState->symbols.end())
		{
			if((*peek)->isOperator || (*peek)->value != "[" || (*peek)->value != "]")
			{
				++peek;
			}
			else
			{
				next = *peek;
				break;
			}
		}
		
		
		Rule const* rule = getRuleForSymbol(context, symbol, next);
		if(rule)
		{
			SymbolList* result = rule->evaluate(context, symbol, next);
			*(output)+= *result;
			delete result;
		}
		// If no rule is found that matches the current symbol, append the original symbol to the output.
		else
		{
			// std::cout << "No rule to match " << symbol->toString() << " so keeping symbol" << std::endl;
			*(output)+= *symbol;
		}
		
		// Add the current symbol to the context.
		context.insert(context.begin(), symbol);
	}
	
	if(mState)
	{
		delete mState;
	}
	
	mState = output;
}

