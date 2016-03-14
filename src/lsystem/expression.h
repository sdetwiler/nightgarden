//
//  expression.h
//
//  Created by Stephen Detwiler on 2/18/16.
//  Copyright © 2016 Pirate Studios. All rights reserved.
//

#ifndef expression_h
#define expression_h

#include <string>

class VariableMap;
class Variable;

////////////////////////////////////////////////////////////////////////////////

class Expression
{
public:
	std::string value;
	
	Expression();
	
	Expression(Expression const& rhs);
	
	~Expression();
	
	std::string toString() const;
	
	bool operator==(Expression const& rhs) const;
	
	bool operator!=(Expression const& rhs) const;
	
	Expression& operator+=(Expression const& rhs);
	
	bool eval(VariableMap* scope, Variable* ret);
};


#endif /* expression_h */
