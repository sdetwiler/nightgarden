//
//  expression.cpp
//
//  Created by Stephen Detwiler on 2/18/16.
//  Copyright © 2016 Pirate Studios. All rights reserved.
//

#include "expression.h"
#include "types.h"

using namespace std;

// Parser forward declarations.
#ifndef FLEXINT_H
typedef struct yy_buffer_state *YY_BUFFER_STATE;

YY_BUFFER_STATE  expression__scan_string(const char *s);
void expression__delete_buffer(YY_BUFFER_STATE buf);

int expression_parse();


#endif

Variable* gExpressionReturnVar = nullptr;
Variable* getExpressionReturnVariable()
{
	return gExpressionReturnVar;
}

VariableMap* gScope = nullptr;
Variable* getVariable(char const* name)
{
	if(gScope)
	{
		string k(name);
		StringVariableMap::iterator i = gScope->variables.find(k);
		if(i!=gScope->variables.end())
		{
			return i->second;
		}
	}
	
	return nullptr;
}


Expression::Expression()
{
}

Expression::Expression(Expression const& rhs)
{
	value = rhs.value;
}

Expression::~Expression()
{
	
}

std::string Expression::toString() const
{
	return value;
}

bool Expression::operator==(Expression const& rhs) const
{
	return value == rhs.value;
}

bool Expression::operator!=(Expression const& rhs) const
{
	return value != rhs.value;
}

Expression& Expression::operator+=(Expression const& rhs)
{
	value+=rhs.value;
	
	return *this;
}

bool Expression::eval(VariableMap* scope, Variable* ret)
{
	// OMG NOT THREAD SAFE.

	gExpressionReturnVar = ret;
	gScope = scope;
	
	YY_BUFFER_STATE buf;
	
	buf = expression__scan_string(value.c_str());
	
	int r = expression_parse();
	
	expression__delete_buffer(buf);
	
	gExpressionReturnVar = nullptr;
	gScope = nullptr;
	
	
	if(r)
	{
		return false;
	}
	
//	cout << "Expression::eval " << value << "=" << ret->toString() << endl;
	
	return true;
}
