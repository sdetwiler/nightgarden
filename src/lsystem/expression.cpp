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
typedef void* yyscan_t;
YY_BUFFER_STATE expression__scan_string (const char* yy_str, yyscan_t yyscanner );
void expression__delete_buffer(YY_BUFFER_STATE  b, yyscan_t yyscanner);
int expression_parse(void* scanner, VariableMap* scope, Variable* retVal);

int expression_lex_init(yyscan_t* scanner);
int expression_lex_destroy(yyscan_t scanner);


#endif

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
	// HACK
//	cout << "Expression::eval \"" << value << "\"\n";
	
	void* scanner;
	
	expression_lex_init (&scanner);
	
	YY_BUFFER_STATE buf = NULL;
	
	buf = expression__scan_string(value.c_str(), scanner);
	
	int r = expression_parse(scanner, scope, ret);
	
	expression__delete_buffer(buf, scanner);
	expression_lex_destroy(scanner);
	
	if(r)
	{
		cout << "yyparse returns " << r << endl;
		return false;
	}
	
	return true;
}
