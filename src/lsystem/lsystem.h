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
	
	void setAxiom(SymbolList const& axiom);
	void setState(SymbolList const& state);
	void addRule(Rule* rule);
	void addGlobalVariable(Variable* variable);
	float getGlobalVariable(char const* name, float def=0) const;
	
	// Loads the system located at filename.
	bool load(char const* filename);
	
	// Parses the string input containing a system.
	bool parse(char const* input);

	// Loads a previously compiled system located at filename. On success numStates contains the number of loaded states.
	bool loadCompiled(char const* filename, size_t* numStates);

	// Compiles the currently loaded system and writes the compiled results to the specified outputFilename.
	bool compile(char const* outputFilename);
	
	void dumpRules() const;
	void dumpVariables() const;
	
	// Clears any data managed by the system.
	void clear();
	
	// Appends the curent state of the system to passed SymbolList state.
	bool getDereferencedState(SymbolList& state) const;

	// Returns the curent state of the system. References to external systems are preserved as reference symbols.
	SymbolList const& getState() const;

	// Claims the currrent system state into passed rhs and removes memory ownership from this system.
	void claimState(SymbolList& rhs);

	// Step the system by time delta dt.
	void step(float dt);
	
	// Remove empty or redundant branches from the provided state.
	void reduce(SymbolList& state);

	// Call to clear the global compiled cache.
	static void clearCompiledCache();

private:
	friend ReferenceRule;
	friend Predicate;
	
	typedef std::vector< Rule* > RuleVec;

	// State if the system is processing in real-time and not compiled.
	RuleVec					mRules;			// All rules.
	VariableMap				mVariables;		// All global variables.
	SymbolList				mAxiom;			// The axiom.
	SymbolList				mState;			// The current system state.
	
	VariableMap const& getGlobalVariables() const;
	
	Rule const* getRuleForSymbol(SymbolStack const& context, Symbol const* symbol, Symbol const* next) const;

	
	// State if the system is operating on previously compiled data.
	size_t					mCurrCompiledState;		// Index of the current compiled state.
	SymbolListVec const*	mStates;				// All compiled states for the system.

	// Compiled state cache.
	typedef std::map<std::string, SymbolListVec> StringSymbolListVecMap;
	static SymbolListVec const* getCompiledStates(char const* name);
	static bool loadCompiled(char const* filename, SymbolListVec& states);
	static StringSymbolListVecMap sCache;

};


#endif /* lsystem_h */
