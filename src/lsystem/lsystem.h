//
//  lsystem.h
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
	
	virtual ~LSystem();
	
	void setAxiom(Result* axiom);
	void setState(Result* state);
	
	bool load(char const* filename);
	bool parse(char const* input);

	bool loadCompiled(char const* filename, size_t* numStates);

	
	bool compile(char const* outputFilename);
	
	void addRule(Rule* rule);
	void dumpRules();

	void addVariable(Variable* variable);
	void dumpVariables();
	VariableMap const& getGlobalVariables() const;
	
	float getGlobalVariable(char const* name, float def=0) const;
	
	void clear();
	
	SymbolList const* getState() const;
	void step(float dt);
	void reduce(SymbolList* state);

private:
	typedef std::vector< Rule* > RuleVec;
	RuleVec		mRules;
	VariableMap mVariables;
	Result*		mAxiom;
	SymbolList const* mState;
	
	
//	typedef std::vector<std::string> StringVec;
//	StringVec mCompiledStates;
	size_t mCurrCompiledState;
	
	SymbolListVec const* mStates;
//	bool mCompiled;

	typedef std::map<std::string, SymbolListVec*> StringSymbolListVecMap;
	
	
	static SymbolListVec const* getCompiledStates(char const* name);
	static SymbolListVec* loadCompiled(char const* filename);
	
	
	Rule const* getRuleForSymbol(SymbolStack const& context, Symbol const* symbol, Symbol const* next);
};


#endif /* lsystem_h */
