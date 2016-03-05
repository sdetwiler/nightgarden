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
#include <stack>
#include <map>
#include <limits>
#include <iostream>

#include "expression.h"

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
	bool isOperator;
	
	VariableList* variables; // (parameters)
	ExpressionList* expressions;
	
	Symbol()
	{
		isOperator = false;
		variables = nullptr;
		expressions = nullptr;
	}
	
	Symbol(Symbol const& rhs)
	{
		isOperator = rhs.isOperator;
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
typedef std::stack< Symbol* > SymbolStack;
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
typedef std::map< std::string, Variable* > StringVariableMap;

class VariableMap
{
public:
	StringVariableMap variables;
	
	
	VariableMap()
	{
	}
	
	VariableMap(VariableMap const& rhs)
	{
		for(StringVariableMap::const_iterator i = rhs.variables.begin(); i!=rhs.variables.end(); ++i)
		{
			Variable* v = new Variable(*(i->second));
			variables[v->name] = v;
		}
	}
	
	~VariableMap()
	{
		clear();
	}
	
	void clear()
	{
		for(StringVariableMap::iterator i = variables.begin(); i!=variables.end(); ++i)
		{
			delete i->second;
		}
	}
	
	std::string toString() const
	{
		std::string s;
		for(StringVariableMap::const_iterator i = variables.begin(); i!=variables.end(); ++i)
		{
			if(s.length())
			{
				s+= ",";
			}
			s+= (i->second)->toString();
		}
		
		return s;
	}
	
	VariableMap& operator+=(VariableMap const& rhs)
	{
		for(StringVariableMap::const_iterator i = rhs.variables.begin(); i!=rhs.variables.end(); ++i)
		{
			Variable* v = new Variable(*(i->second));
			variables[v->name] = v;
		}
		
		
		return *this;
	}
	
	VariableMap& operator+=(Variable const& rhs)
	{
		Variable* v = new Variable(rhs);
		variables[v->name] = v;
		
		return *this;
	}
	
	
	size_t addToMap(VariableList* variables, Symbol const* s, size_t idx=0)
	{
		for(VariableVec::iterator vi = variables->variables.begin(); vi!= variables->variables.end(); ++vi)
		{
			std::cout << (*vi)->name << "=" << (*vi)->value << std::endl;
			Variable* variable = new Variable(*(*vi));
			if(s->expressions)
			{
				if(idx < s->expressions->expressions.size())
				{
					Expression* e = s->expressions->expressions.at(idx);
					
					if(e->eval(this, variable))
					{
						this->variables[variable->name] = variable;
					}
					else
					{
						std::cout << "Failed to evaluate expression " << e->toString() << std::endl << "With scope " << toString() << std::endl;
					}
				}
				else
				{
					std::cout << "no expressions at idx " << idx << " for " << s->toString() << std::endl << "only has " << s->expressions->expressions.size() << " expressions" << std::endl;
				}
			}
			else
			{
				std::cout << "no expresions" << std::endl;
			}
			++idx;
		}
		
		return idx;
	}
};






#endif /* types_h */
