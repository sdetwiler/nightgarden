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

////////////////////////////////////////////////////////////////////////////////
class Symbol
{
public:
	std::string value;
	// TODO parameters...
	
	Symbol()
	{
		
	}
	
	Symbol(Symbol const& rhs)
	{
		value = rhs.value;
	}
	
	~Symbol()
	{
		
	}
	
	std::string toString() const
	{
		return value;
	}
	
	bool operator==(Symbol const& rhs) const
	{
		return value == rhs.value;
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
};


////////////////////////////////////////////////////////////////////////////////
class Predicate
{
public:
	Symbol* symbol;
	
	Predicate()
	{
		symbol = nullptr;
	}
	
	~Predicate()
	{
		if(symbol)
		{
			delete symbol;
		}
	}
	
	std::string toString() const
	{
		if(symbol)
		{
			return symbol->toString();
		}
		else
		{
			return "null";
		}
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
