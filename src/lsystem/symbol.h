//
//  symbol.h
//  nightgarden
//
//  Created by Stephen Detwiler on 3/21/16.
//
//

#ifndef symbol_h
#define symbol_h

#include <string>
#include <stack>
#include <vector>
#include <forward_list>
#include <iostream>

class VariableList;
class ExpressionList;

////////////////////////////////////////////////////////////////////////////////
class Symbol
{
public:
	std::string value;				// The symbol value.
	bool isOperator;				// True if the symbol is an lsystem turtle operator.
	
	bool isTerminal;				// Has the symbol reached it's terminal age.
	float terminalAge;				// The symbol's terminal age.
	float age;						// The symbol's current age.
	
	VariableList* variables;		// parameter variable names in Predicate.
	ExpressionList* expressions;	// Parameter variable values in Result.
	
	Symbol();
	Symbol(Symbol const& rhs);
	
	~Symbol();
	
	// Returns lsystem string encoding of symbol.
	std::string toString() const;
	
	// Returns timed lsystem string encoding of symbol.
	std::string toTimedString() const;

	Symbol& assign(Symbol const& rhs);

	bool operator==(Symbol const& rhs) const;
	bool operator!=(Symbol const& rhs) const;
	
	Symbol& operator=(Symbol const& rhs);
	
	// Use the symbol's current age to scale the value v by a growth function.
	// Used to animate the current symbol.
	float applyGrowthFunction(float v) const;
	float linearGrowthFunction(float v) const;
};


typedef std::stack< Symbol* > SymbolStack;

//typedef std::forward_list< Symbol > SymbolList;

class SymbolList : public std::forward_list< Symbol >
{
public:
	SymbolList()
	{
		mLast = before_begin();
	}
	
	std::string toOperatorTimedString() const
	{
		std::string s;
		for(SymbolList::const_iterator i = begin(); i!=end(); ++i)
		{
			if((*i).isOperator)
			{
				s+= (*i).toTimedString();
			}
		}
		
		return s;
	}
	
	std::string toTimedString() const
	{
		std::string s;
		for(SymbolList::const_iterator i = begin(); i!=end(); ++i)
		{
			s+= (*i).toTimedString();
		}
		
		return s;
	}
	
	std::string toString() const
	{
		std::string s;
		for(SymbolList::const_iterator i = begin(); i!=end(); ++i)
		{
			s+= (*i).toString();
		}
		
		return s;
	}

	void append(Symbol const& symbol)
	{
		// FIXME Need to cache this value... 
		SymbolList::iterator last;
		last = before_begin();
		std::advance(last, std::distance(begin(), end()));

		insert_after(last, symbol);
	}
	
	void claim(SymbolList& rhs)
	{
		clear();
		splice_after(before_begin(), rhs);
	}
	
private:
	SymbolList::iterator mLast;
};

typedef std::vector< SymbolList >SymbolListVec;

#endif /* symbol_h */
