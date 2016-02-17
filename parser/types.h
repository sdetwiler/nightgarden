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

class Expression
{
public:
	std::string value;
	
	Expression()
	{
	}
	
	Expression(Expression const& rhs)
	{
		value = rhs.value;
	}
	
	~Expression()
	{
		
	}
	
	std::string toString() const
	{
		return value;
	}
	
	bool operator==(Expression const& rhs) const
	{
		return value == rhs.value;
	}
	
	bool operator!=(Expression const& rhs) const
	{
		return value != rhs.value;
	}
	
	Expression& operator+=(Expression const& rhs)
	{
		value+=rhs.value;
		
		return *this;
	}
	
};

////////////////////////////////////////////////////////////////////////////////
typedef std::vector< Expression* > ExpressionVec;
class ExpressionList
{
public:
	ExpressionVec expressions;
	
	
	ExpressionList()
	{
	}
	
	ExpressionList(ExpressionList const& rhs)
	{
		for(ExpressionVec::const_iterator i = rhs.expressions.begin(); i!=rhs.expressions.end(); ++i)
		{
			Expression* s = new Expression(*(*i));
			expressions.push_back(s);
		}
		
	}
	
	~ExpressionList()
	{
		for(ExpressionVec::iterator i = expressions.begin(); i!=expressions.end(); ++i)
		{
			delete (*i);
		}
	}
	
	std::string toString() const
	{
		std::string s;
		for(ExpressionVec::const_iterator i = expressions.begin(); i!=expressions.end(); ++i)
		{
			if(s.length())
			{
				s+= ",";
			}
			s+= (*i)->value;
		}
		
		return s;
	}
	
	ExpressionList& operator+=(ExpressionList const& rhs)
	{
		for(ExpressionVec::const_iterator i = rhs.expressions.begin(); i!=rhs.expressions.end(); ++i)
		{
			expressions.push_back(new Expression(*(*i)));
		}
		
		
		return *this;
	}
	
	ExpressionList& operator+=(Expression const& rhs)
	{
		expressions.push_back(new Expression(rhs));
		
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
	
	ExpressionList* expressions;
	
	Symbol()
	{
		variables = nullptr;
		expressions = nullptr;
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
		if(rhs.expressions)
		{
			expressions = new ExpressionList(*(rhs.expressions));
		}
		else
		{
			expressions = nullptr;
		}

	}
	
	~Symbol()
	{
		if(variables)
		{
			delete variables;
		}
		if(expressions)
		{
			delete expressions;
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

		if(expressions && expressions->expressions.size())
		{
			
			s+= "(";
			s+= expressions->toString();
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
			s+= (*i)->toString();
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
	
	Result(Result const& rhs)
	{
		if(rhs.symbolList)
		{
			symbolList = new SymbolList(*rhs.symbolList);
		}
		else
		{
			symbolList = nullptr;
		}
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
	
	SymbolList* evaluate(Symbol* s) const
	{
		std::cout << "Rule::evaluate" << std::endl
					<< "rule: " << this->toString() << std::endl
					<< "smbl: " << s->toString() << std::endl;
		
		
		// If the predicate's symbol has attached variables...
		if(predicate->symbol->variables)
		{
			
			// Map variable names in rule to values in symbol.
			for(VariableVec::iterator vi = predicate->symbol->variables->variables.begin(); vi!= predicate->symbol->variables->variables.end(); ++vi)
			{
				std::cout << (*vi)->name << std::endl;
			}
			
		}
		
		SymbolList* res = new SymbolList(*(result->symbolList));
		
		std::cout << "rslt: " << res->toString() << std::endl << std::endl;
		return res;
	}
};







#endif /* types_h */
