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
#include "types.h"


class LSystem
{
public:
	LSystem();
	
	~LSystem();

	
	static LSystem& getInstance();
	
	void setAxiom(SymbolList* axiom);
	
	void parse(char const* input);

	void addRule(Rule* rule);

	void dumpRules();
	
	void clear();
	
	SymbolList const* getState();
	void step();
	
private:
	typedef std::vector< Rule* > RuleVec;
	RuleVec mRules;
	
	SymbolList* mAxiom;
	
	SymbolList* mState;
	
	Rule const* getRuleForSymbol(Symbol const& s);


	
};


#endif /* lsystem_h */
