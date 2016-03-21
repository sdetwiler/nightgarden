//
//  variable.h
//  nightgarden
//
//  Created by Stephen Detwiler on 3/21/16.
//
//

#ifndef variable_h
#define variable_h

#include <string>

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
	
	Variable();
	
	Variable(Variable const& rhs);
	
	~Variable();
	
	std::string toString() const;
	
	Variable& operator=(Variable const& rhs);
	Variable& operator=(float rhs);
	Variable& operator=(double rhs);
	Variable& operator=(int rhs);
	Variable& operator=(char const* rhs);
	Variable& operator=(std::string const& rhs);

	Variable operator+(Variable const& rhs);
	Variable operator-(Variable const& rhs);
	Variable operator*(Variable const& rhs);
	Variable operator/(Variable const& rhs);

	bool operator==(Variable const& rhs) const;
	bool operator!=(Variable const& rhs) const;
	bool operator<(Variable const& rhs) const;
	bool operator<=(Variable const& rhs) const;
	bool operator>(Variable const& rhs) const;
	bool operator>=(Variable const& rhs) const;
};

#endif /* variable_h */
