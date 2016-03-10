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

#include "timing.h"



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
	
	return parse(data.c_str());
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

float LSystem::getGlobalVariable(char const* name, float def) const
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


SymbolList const* LSystem::getState() const
{
	return mState;
}


Rule const* LSystem::getRuleForSymbol(SymbolStack const& context, Symbol const* symbol, Symbol const* next)
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

void LSystem::reduce()
{
	if(!mState)
	{
		return;
	}

	typedef std::stack<SymbolVec*> SymbolVecStack;
	
	SymbolVecStack frames;
	SymbolVec toRemove;
	
	
	SymbolVec* currFrame = new SymbolVec();
	frames.push(currFrame);
	
	for(SymbolVec::const_iterator i = mState->symbols.begin(); i!=mState->symbols.end(); ++i)
	{
		Symbol* symbol = (*i);
		
		if(symbol->value == "[")
		{
			currFrame = new SymbolVec();
			frames.push(currFrame);
			currFrame->push_back(symbol);
		}
		else if(symbol->value == "]")
		{
			// Check frame contents and if only [ then drop those symbols from the symbollist.
			if(currFrame->size() == 1 && currFrame->at(0)->value == "[")
			{
				toRemove.push_back(currFrame->at(0));
				toRemove.push_back(symbol);
			}
			delete currFrame;
			frames.pop();
			currFrame = frames.top();
		}
		else
		{
			currFrame->push_back(symbol);
		}
	}
	
	for(SymbolVec::iterator i = toRemove.begin(); i != toRemove.end(); ++i)
	{
		
		SymbolVec::iterator j = std::find(mState->symbols.begin(), mState->symbols.end(), *i);
		if(j!=mState->symbols.end())
		{
			mState->symbols.erase(j);
		}
	}

	while(frames.size())
	{
		delete frames.top();
		frames.pop();
	}
	
//	std::cout << "Removed " << std::to_string(toRemove.size()) << " symbols\n";
}

void LSystem::step()
{
	bool reduce = true;
	
	START_TIME_MEASURE(step);
	
	if(!mState)
	{
		return;
	}
	SymbolList* output = new SymbolList;

	SymbolStack context;
	Symbol root;
	context.push(&root);
	SymbolStack branches;
	
//	std::cout << " LSystem::step processing " << std::to_string(mState->symbols.size()) << " symbols\n";
	for(SymbolVec::const_iterator i = mState->symbols.begin(); i!=mState->symbols.end(); ++i)
	{
		START_TIME_MEASURE(itr);

		Symbol* symbol = (*i);

		// If a push branch symbol...
		if(symbol->value == "[")
		{
			// Add the symbol to the output.
			*(output)+= *symbol;

			// Store the current head of the context in the branches vector.
			branches.push(context.top());
		}
		
		// If a pop branch symbol...
		else if(symbol->value == "]")
		{
			// Add the symbol to the output.
			*(output)+= *symbol;

			// Get and pop location of the last branch and rewind the context back to that point.
			if(branches.size() == 0)
			{
				std::cout << "WARNING: Mismatch square brackets";
				context = SymbolStack();
			}
			else
			{
				Symbol* b = branches.top();
				branches.pop();
				while(context.size())
				{
					if(b == context.top())
					{
						break;
					}
					context.pop();
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
		size_t peekDistance = 0;

//////////////////////////////////////////////////////
// TODO FIXME This is all broken and currently disabled.
//
//		START_TIME_MEASURE(peek_timer);
//
//		while(peek != mState->symbols.end())
//		{
//			++peekDistance;
////			if((*peek)->isOperator || (*peek)->value != "[" || (*peek)->value != "]")
////			{
////				++peek;
////			}
////			else
////			{
//				next = *peek;
//				break;
////			}
//		}
//		LOG_TIME_DELTA(peek_timer, "\t\tpeek");
//		
//		std::cout << "next is " << (next?next->toString():"null") << " scanned ahead " << std::to_string(peekDistance) << " symbols\n";
//////////////////////////////////////////////////////
		
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
		context.push(symbol);

		LOG_TIME_DELTA(itr, "\titrend");

	}
	
	if(mState)
	{
		delete mState;
	}
	
	mState = output;
	
	LOG_TIME_DELTA(step, "end");
}

