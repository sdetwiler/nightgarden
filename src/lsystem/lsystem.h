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
	
	bool load(char const* filename);
	bool parse(char const* input);

	void addRule(Rule* rule);
	void dumpRules();

	void addVariable(Variable* variable);
	void dumpVariables();
	VariableMap const& getGlobalVariables();
	
	float getGlobalVariable(char const* name, float def=0);
	
	void clear();
	
	SymbolList const* getState();
	void step();
	void reduce();
	
private:
	typedef std::vector< Rule* > RuleVec;
	RuleVec mRules;

	VariableMap mVariables;
	
	Result* mAxiom;
	
	SymbolList* mState;
	
	Rule const* getRuleForSymbol(SymbolStack const& context, Symbol const* symbol, Symbol const* next);
	

};


#endif /* lsystem_h */
