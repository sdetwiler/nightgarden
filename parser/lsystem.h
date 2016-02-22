//
//  lsystem.h
//  parser
//
//  Created by Stephen Detwiler on 2/15/16.
//  Copyright © 2016 Pirate Studios. All rights reserved.
//

#ifndef lsystem_h
#define lsystem_h

#include <stdio.h>

#include <vector>
#include <map>
#include "types.h"
#include "rule.h"

class LSystem
{
public:
	LSystem();
	
	~LSystem();

	
	static LSystem& getInstance();
	
	void setAxiom(Result* axiom);
	
	bool parse(char const* input);

	void addRule(Rule* rule);
	void dumpRules();

	void addVariable(Variable* variable);
	void dumpVariables();
	VariableMap const& getGlobalVariables();
	
	void clear();
	
	SymbolList const* getState();
	void step();
	
private:
	typedef std::vector< Rule* > RuleVec;
	RuleVec mRules;

	VariableMap mVariables;
	
	Result* mAxiom;
	
	SymbolList* mState;
	
	Rule const* getRuleForSymbol(SymbolVec const& context, Symbol const* symbol, Symbol const* next);
};


#endif /* lsystem_h */
