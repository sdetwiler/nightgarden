//
//  types.h
//  parser
//
//  Created by Stephen Detwiler on 2/13/16.
//  Copyright © 2016 Pirate Studios. All rights reserved.
//

#ifndef types_hpp
#define types_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <limits>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
class Variable
{
public:
	std::string name;
	float value;
	bool constant;
	
	Variable()
	{
		constant = false;
		value = std::numeric_limits<float>::quiet_NaN();
	}
	
	Variable(Variable const& rhs)
	{
		name = rhs.name;
		value = rhs.value;
		constant = rhs.constant;
	}
	
	~Variable()
	{
		
	}
	
	std::string toString() const
	{
		std::string s;
		s+= name;
		s+= ": ";
		if(constant)
		{
			s+="(constant)";
		}
		else
		{
			s+= std::to_string(value);
		}
		return s;
	}
	
	bool operator==(Variable const& rhs) const
	{
		return value == rhs.value;
	}
	
	bool operator!=(Variable const& rhs) const
	{
		return value != rhs.value;
	}
};

////////////////////////////////////////////////////////////////////////////////
typedef std::vector< Variable* > VariableVec;
class VariableList
{
public:
	VariableVec variables;
	
	
	VariableList()
	{
	}
	
	VariableList(VariableList const& rhs)
	{
		for(VariableVec::const_iterator i = rhs.variables.begin(); i!=rhs.variables.end(); ++i)
		{
			Variable* s = new Variable(*(*i));
			variables.push_back(s);
		}
		
	}
	
	~VariableList()
	{
		for(VariableVec::iterator i = variables.begin(); i!=variables.end(); ++i)
		{
			delete (*i);
		}
	}
	
	std::string toString() const
	{
		std::string s;
		for(VariableVec::const_iterator i = variables.begin(); i!=variables.end(); ++i)
		{
			if(s.length())
			{
				s+= ",";
			}
			s+= (*i)->toString();
		}
		
		return s;
	}
	
	VariableList& operator+=(VariableList const& rhs)
	{
		for(VariableVec::const_iterator i = rhs.variables.begin(); i!=rhs.variables.end(); ++i)
		{
			variables.push_back(new Variable(*(*i)));
		}
		
		
		return *this;
	}
	
	VariableList& operator+=(Variable const& rhs)
	{
		variables.push_back(new Variable(rhs));
		
		return *this;
	}
	
};



////////////////////////////////////////////////////////////////////////////////
class Symbol
{
public:
	std::string value;
	VariableList* variables; // (parameters)
	// TODO parameters...
	
	Symbol()
	{
		variables = nullptr;
	}
	
	Symbol(Symbol const& rhs)
	{
		value = rhs.value;
		if(rhs.variables)
		{
			variables = new VariableList(*(rhs.variables));
		}
		else
		{
			variables = nullptr;
		}
	}
	
	~Symbol()
	{
		if(variables)
		{
			delete variables;
		}
		
	}
	
	std::string toString() const
	{
		std::string s;
		s+= value;
		if(variables && variables->variables.size())
		{
			s+= "(";
			
			s+= variables->toString();
			
			s+= ")";
		}
		return s;
	}
	
	bool operator==(Symbol const& rhs) const
	{
		return value == rhs.value;
	}

	bool operator!=(Symbol const& rhs) const
	{
		return value != rhs.value;
	}
};


////////////////////////////////////////////////////////////////////////////////
typedef std::vector< Symbol* > SymbolVec;
class SymbolList
{
public:
	SymbolVec symbols;

	
	SymbolList()
	{
	}
	
	SymbolList(SymbolList const& rhs)
	{
		for(SymbolVec::const_iterator i = rhs.symbols.begin(); i!=rhs.symbols.end(); ++i)
		{
			Symbol* s = new Symbol(*(*i));
			symbols.push_back(s);
		}
		
	}
	
	~SymbolList()
	{
		for(SymbolVec::iterator i = symbols.begin(); i!=symbols.end(); ++i)
		{
			delete (*i);
		}
	}
	
	std::string toString() const
	{
		std::string s;
		for(SymbolVec::const_iterator i = symbols.begin(); i!=symbols.end(); ++i)
		{
			s+= (*i)->value;
		}
		
		return s;
	}
	
	SymbolList& operator+=(SymbolList const& rhs)
	{
		for(SymbolVec::const_iterator i = rhs.symbols.begin(); i!=rhs.symbols.end(); ++i)
		{
			symbols.push_back(new Symbol(*(*i)));
		}
		
		
		return *this;
	}

	SymbolList& operator+=(Symbol const& rhs)
	{
		symbols.push_back(new Symbol(rhs));
		
		return *this;
	}

};


////////////////////////////////////////////////////////////////////////////////
class Predicate
{
public:
	Symbol* symbol;
	Symbol* prev;	// Previous symbol for context-sensitive evaluation.
	Symbol* next;	// Next symbol for context-sensitive evaluation.
	
	Predicate()
	{
		symbol = nullptr;
		prev = nullptr;
		next = nullptr;
	}
	
	~Predicate()
	{
		if(symbol)
		{
			delete symbol;
		}
		
		if(prev)
		{
			delete prev;
		}
		
		if(next)
		{
			delete next;
		}
	}
	
	std::string toString() const
	{
		std::string s;
		
		if(prev)
		{
			s+= prev->toString();
			s+= "<";
		}
		
		if(symbol)
		{
			s+=symbol->toString();
		}

		if(next)
		{
			s+= ">";
			s+= next->toString();
		}
		
		return s;
	}
	
	bool doesMatch(Symbol const* prevSymbol, Symbol const* currSymbol, Symbol const* nextSymbol)
	{
		if(prev)
		{
			if(prevSymbol)
			{
				if(*prev != *prevSymbol)
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}

		if(next)
		{
			if(nextSymbol)
			{
				if(*next != *nextSymbol)
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		
		if(*symbol != *currSymbol)
		{
			return false;
		}
		
		return true;
	}
};


////////////////////////////////////////////////////////////////////////////////
class Result
{
public:
	SymbolList* symbolList;
	
	Result()
	{
		symbolList = nullptr;
	}
	
	~Result()
	{
		if(symbolList)
		{
			delete symbolList;
		}
	}
	
	std::string toString() const
	{
		if(symbolList)
		{
			return symbolList->toString();
		}
		else
		{
			return "null";
		}
	}
};


////////////////////////////////////////////////////////////////////////////////
class Rule
{
public:
	Predicate* predicate;
	Result* result;

	Rule()
	{
		predicate = nullptr;
		result = nullptr;
	}
	
	~Rule()
	{
		if(predicate)
		{
			delete predicate;
		}
		
		if(result)
		{
			delete result;
		}
	}
	
	std::string toString() const
	{
		std::string s;
		
		s+= predicate->toString();
		s+= "->";
		s+= result->toString();
		
		return s;
	}
};







#endif /* types_h */
