//
//  lsystem.cpp
//
//  Created by Stephen Detwiler on 2/15/16.
//  Copyright © 2016 Pirate Studios. All rights reserved.
//

#include "lsystem.h"
#include "predicate.h"

#include <iostream>
#include <fstream>
#include <stack>
#include <map>

#include "rules_parser.tab.h"

#include "timing.h"

using namespace std;

// Parser forward declarations.
#ifndef FLEXINT_H

typedef struct yy_buffer_state *YY_BUFFER_STATE;
typedef void* yyscan_t;
YY_BUFFER_STATE rules__scan_string (const char* yy_str, yyscan_t yyscanner );
void rules__delete_buffer(YY_BUFFER_STATE  b, yyscan_t yyscanner);
int rules_parse(void* scanner, LSystem* lsystem);

int rules_lex_init(yyscan_t* scanner);
int rules_lex_destroy(yyscan_t scanner);


#endif


SymbolListVec* LSystem::loadCompiled(char const* filename)
{
	ifstream infile(filename);
	if(!infile.is_open())
	{
		cout << "Failed to open " << filename << endl;
		return nullptr;
	}

	SymbolListVec* states = new SymbolListVec;

	LSystem system;
	string line;
	while(getline(infile, line))
	{
		line += "\n";
		if(system.parse(line.c_str()))
		{
			// FIXME Blarg so many memory copies.
			SymbolList* state = new SymbolList(*system.getState());
			states->push_back(state);
		}
	}
	
	if(infile.bad())
	{
		cout << "Error while reading from " << filename << endl;
		return nullptr;
	}
	
	return states;
}


// TODO This is a big, fat memory leak.
SymbolListVec const* LSystem::getCompiledStates(char const* name)
{
	static StringSymbolListVecMap cache;
	
	SymbolListVec* states;
	
	StringSymbolListVecMap::iterator i = cache.find(name);
	if(i == cache.end())
	{
		states = LSystem::loadCompiled(name);
		if(states)
		{
			cache[name] = states;
		}
	}
	else
	{
		states = i->second;
	}
	
	return states;
}


LSystem::LSystem()
{
	mAxiom = nullptr;
	mState = nullptr;
}

LSystem::~LSystem()
{
	clear();
}

bool LSystem::load(char const* filename)
{
	clear();
	
	ifstream infile(filename);
	if(!infile.is_open())
	{
		cout << "Failed to open " << filename << endl;
		return -1;
	}
	string data;
	string line;
	while(getline(infile, line))
	{
		data+=line;
		data+="\n";
	}
	
	if(infile.bad())
	{
		cout << "Error while reading from " << filename << endl;
		return false;
	}
	
	return parse(data.c_str());
}

bool LSystem::parse(char const* input)
{
	void* scanner;
	
	rules_lex_init(&scanner);

	YY_BUFFER_STATE buf = NULL;
	
	buf = rules__scan_string(input, scanner );
	
	int ret = rules_parse(scanner, this);
	
	rules__delete_buffer(buf, scanner);
	rules_lex_destroy(scanner);

	if(ret)
	{
		return false;
	}

	return true;
}



bool LSystem::loadCompiled(char const* filename, size_t* numStates)
{
	clear();
	
	mStates = LSystem::getCompiledStates(filename);
	if(mStates && numStates)
	{
		*numStates = mStates->size();
	}
		
	return mStates != nullptr;
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

void LSystem::setState(Result* state)
{
	if(mState)
	{
		delete mState;
	}
	
	mState = state->symbolList;
	state->symbolList = nullptr;
	delete state;
}

void LSystem::addRule(Rule* rule)
{
	mRules.push_back(rule);
}

void LSystem::dumpRules()
{
	cout << "LSystem::dumpRules" << endl;
	for(RuleVec::iterator i = mRules.begin(); i!=mRules.end(); ++i)
	{
		cout << (*i)->toString() << endl;
	}
	cout << endl;
	
}

void LSystem::addVariable(Variable* variable)
{
	mVariables.variables[variable->name] = variable;
}

void LSystem::dumpVariables()
{
	cout << "LSystem::dumpVariables" << endl;
	cout << mVariables.toString() << endl;
	cout << endl;
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
	
	mCurrCompiledState = 0;
	mStates = nullptr;
}

float LSystem::getGlobalVariable(char const* name, float def) const
{
	float ret = def;
	StringVariableMap::const_iterator i;
	i = mVariables.variables.find(string(name));
	if(i!=mVariables.variables.end())
	{
		ret = i->second->value;
	}
	return ret;
}


VariableMap const& LSystem::getGlobalVariables() const
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

void LSystem::reduce(SymbolList* state)
{
	if(!state)
	{
		return;
	}

	typedef stack<SymbolVec*> SymbolVecStack;
	
	SymbolVecStack frames;
	SymbolVec toRemove;
	
	
	SymbolVec* currFrame = new SymbolVec();
	frames.push(currFrame);
	
	for(SymbolVec::const_iterator i = state->symbols.begin(); i!=state->symbols.end(); ++i)
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
		
		SymbolVec::iterator j = find(state->symbols.begin(), state->symbols.end(), *i);
		if(j!=state->symbols.end())
		{
			state->symbols.erase(j);
		}
	}

	while(frames.size())
	{
		delete frames.top();
		frames.pop();
	}
	
//	cout << "Removed " << to_string(toRemove.size()) << " symbols\n";
}

void LSystem::step(float dt)
{
	if(mStates)
	{
		if(mCurrCompiledState < mStates->size())
		{
			mState = (*mStates)[mCurrCompiledState];
			++mCurrCompiledState;
		}
		
		return;
	}
	
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
	
//	cout << " LSystem::step processing " << to_string(mState->symbols.size()) << " symbols\n";
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
				cout << "WARNING: Mismatch square brackets";
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
//		cout << "next is " << (next?next->toString():"null") << " scanned ahead " << to_string(peekDistance) << " symbols\n";
//////////////////////////////////////////////////////
		
		symbol->age+=dt;

		// Age the symbol by the delta time.
		if(symbol->age > symbol->terminalAge && !symbol->isTerminal)
		{
			symbol->isTerminal = true;
		
			Rule const* rule = getRuleForSymbol(context, symbol, next);
			// Rule exists, evaluate it and replace current symbol with the results.
			if(rule)
			{
				SymbolList* result = rule->evaluate(context, symbol, next);
				*(output)+= *result;
				delete result;
			}
			// If no rule is found that matches the current symbol, append the original symbol to the output.
			else
			{
				// cout << "No rule to match " << symbol->toString() << " so keeping symbol" << endl;
				*(output)+= *symbol;
			}
		}
		// Not yet terminal so keep the current symbol.
		else
		{
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
	
	reduce(output);

	mState = output;
	
	LOG_TIME_DELTA(step, "end");
}



bool LSystem::compile(char const* outputFilename)
{
	cout << "LSystem::compile" << endl;
	
	float stepInterval;
	int maxSteps;
	
	// Is the system configured to step through time, or over an explicit number of steps.
	float duration = getGlobalVariable("duration", -1);
	if(duration > 0)
	{
		stepInterval = getGlobalVariable("stepInterval", 1);
		maxSteps = duration/stepInterval;
	}
	else
	{
		stepInterval = getGlobalVariable("stepInterval", -1);
		if(stepInterval < 0)
		{
			stepInterval = 1/30.;
		}
		maxSteps = getGlobalVariable("steps", 4) / stepInterval;
	}
	
	int currSteps = 0;
	float lastStepTime = 0;

	cout << "\tComputing " << maxSteps << " steps with stepInterval of " << stepInterval << endl;
	cout << "\n\tSaving to " << outputFilename << endl;

	ofstream outfile(outputFilename);
	
	for(int i=0; i<maxSteps; ++i)
	{
		cout << ".";
		cout.flush();
		step(stepInterval);
		outfile << "state " << getState()->toTimedString() << endl;
	}

	outfile.close();

	cout << endl;
	cout << "\tDone\n";
	cout.flush();
	return true;
}
