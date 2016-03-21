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
using namespace std;

////////////////////////////////////////////////////////////////////////////////
class Variable
{
public:
	enum Type
	{
		Float,
		String
	};
	
	std::string name;
	float value;
	std::string stringValue;
	Type type;
	
	Variable()
	{
		value = std::numeric_limits<float>::quiet_NaN();
		type= Float;
	}
	
	Variable(Variable const& rhs)
	{
		name = rhs.name;
		value = rhs.value;
		stringValue = rhs.stringValue;
		type = rhs.type;
	}
	
	~Variable()
	{
		
	}
	
	std::string toString() const
	{
		std::string s;
		s+= name;
		s+= ": ";
		if(type == Float)
		{
			s+= std::to_string(value);
		}
		else if(type == String)
		{
			s+= stringValue;
		}
		return s;
	}
	
	Variable& operator=(Variable const& rhs)
	{
		name = rhs.name;
		value = rhs.value;
		stringValue = rhs.stringValue;
		type = rhs.type;
		
		return *this;
	}

	Variable& operator=(float rhs)
	{
		value = rhs;
		type = Variable::Float;
		
		return *this;
	}

	Variable& operator=(double rhs)
	{
		value = rhs;
		type = Variable::Float;
		
		return *this;
	}

	Variable& operator=(int rhs)
	{
		value = rhs;
		type = Variable::Float;
		
		return *this;
	}

	Variable& operator=(char const* rhs)
	{
		stringValue = rhs;
		type = Variable::String;
		
		return *this;
	}

	Variable& operator=(std::string const& rhs)
	{
		stringValue = rhs;
		type = Variable::String;
		
		return *this;
	}
	
	
	Variable operator+(Variable const& rhs)
	{
		Variable r;
		r.type = type;
		
		if(type == rhs.type)
		{
			if(type == Float)
			{
				r.value = value + rhs.value;
			}
			else if(type == String)
			{
				r.stringValue = stringValue + rhs.stringValue;
			}
		}
		
		return r;
	}

	Variable operator-(Variable const& rhs)
	{
		Variable r;
		r.type = type;
		
		if(type == rhs.type)
		{
			if(type == Float)
			{
				r.value = value - rhs.value;
			}
		}
		
		return r;
	}
	
	Variable operator*(Variable const& rhs)
	{
		Variable r;
		r.type = type;
		
		if(type == rhs.type)
		{
			if(type == Float)
			{
				r.value = value * rhs.value;
			}
		}
		
		return r;
	}

	Variable operator/(Variable const& rhs)
	{
		Variable r;
		r.type = type;
		
		if(type == rhs.type)
		{
			if(type == Float)
			{
				r.value = value / rhs.value;
			}
		}
		
		return r;
	}
	
	bool operator==(Variable const& rhs) const
	{
		if(type != rhs.type)
		{
			return false;
		}
		if(type == Float)
		{
			return value == rhs.value;
		}
		else if(type == String)
		{
			return stringValue == rhs.stringValue;
		}
		
		return false;
	}
	
	bool operator!=(Variable const& rhs) const
	{
		return !(*this == rhs);
	}
	
	
	bool operator <(Variable const& rhs) const
	{
		if(type != rhs.type)
		{
			return false;
		}
		if(type == Float)
		{
			return value < rhs.value;
		}
		else if(type == String)
		{
			return stringValue < rhs.stringValue;
		}
		
		return false;
	}

	bool operator <=(Variable const& rhs) const
	{
		if(type != rhs.type)
		{
			return false;
		}
		if(type == Float)
		{
			return value <= rhs.value;
		}
		else if(type == String)
		{
			return stringValue <= rhs.stringValue;
		}
		
		return false;
	}
	
	bool operator >(Variable const& rhs) const
	{
		if(type != rhs.type)
		{
			return false;
		}
		if(type == Float)
		{
			return value > rhs.value;
		}
		else if(type == String)
		{
			return stringValue > rhs.stringValue;
		}
		
		return false;
	}
	
	bool operator >=(Variable const& rhs) const
	{
		if(type != rhs.type)
		{
			return false;
		}
		if(type == Float)
		{
			return value >= rhs.value;
		}
		else if(type == String)
		{
			return stringValue >= rhs.stringValue;
		}
		
		return false;
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
	
	bool isTerminal;
	float terminalAge;
	float age;
	
	VariableList* variables; // (parameters)
	ExpressionList* expressions;
	
	Symbol()
	{
		isOperator = false;
		variables = nullptr;
		expressions = nullptr;
		age = 0;
		terminalAge = 1;
		isTerminal = false;
	}
	
	Symbol(Symbol const& rhs)
	{
		isOperator = rhs.isOperator;
		value = rhs.value;
		terminalAge = rhs.terminalAge;
		age = rhs.age;
		isTerminal = rhs.isTerminal;
				
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
	
	std::string toTimedString() const
	{
		std::string s;
		s+= "(";
		
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
		
		s+=", ";
		s+=to_string(age);
		s+=", ";
		s+=to_string(terminalAge);
		s+=")";
		
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
	
	// Use the symbol's current age to scale the value v by a growth function.
	// Used to animate the current symbol.
	float applyGrowthFunction(float v)
	{
		// TODO: Implement more than linear growth functions.
		return linearGrowthFunction(v);
	}

	float linearGrowthFunction(float v)
	{
		float max = 1.0;
		float pct = age/terminalAge;
		if(pct > max)
		{
			return v*max;
		}
		return v*pct;
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

	std::string toOperatorTimedString() const
	{
		std::string s;
		for(SymbolVec::const_iterator i = symbols.begin(); i!=symbols.end(); ++i)
		{
			if((*i)->isOperator)
			{
				s+= (*i)->toTimedString();
			}
		}
		
		return s;
	}

	std::string toTimedString() const
	{
		std::string s;
		for(SymbolVec::const_iterator i = symbols.begin(); i!=symbols.end(); ++i)
		{
			s+= (*i)->toTimedString();
		}
		
		return s;
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

	void clear()
	{
		for(SymbolVec::iterator i = symbols.begin(); i!=symbols.end(); ++i)
		{
			delete *i;
		}
		symbols.clear();
	}
	
	SymbolList& operator=(SymbolList const& rhs)
	{
		clear();
		for(SymbolVec::const_iterator i = rhs.symbols.begin(); i!=rhs.symbols.end(); ++i)
		{
			symbols.push_back(new Symbol(*(*i)));
		}
		
		
		return *this;
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

typedef std::vector< SymbolList* >SymbolListVec;


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

	std::string toTimedString() const
	{
		if(symbolList)
		{
			return symbolList->toTimedString();
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
		
		variables.clear();
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
//		cout << "VariableMap::addToMap\nvariables[" << variables->variables.size() << "]:" << variables->toString() << "\nSymbol: " << s->toString() << endl;
		for(VariableVec::iterator vi = variables->variables.begin(); vi!= variables->variables.end(); ++vi)
		{
//			std::cout << (*vi)->name << "=" << (*vi)->value << std::endl;
			Variable* variable = new Variable(*(*vi));
			if(s->expressions)
			{
				if(idx < s->expressions->expressions.size())
				{
					Expression* e = s->expressions->expressions.at(idx);
					
//					cout << "\teval: " << e->toString() << endl;
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
			//	std::cout << "no expresions" << std::endl;
			}
			++idx;
		}
		
		return idx;
	}
};






#endif /* types_h */
