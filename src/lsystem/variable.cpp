//
//  variable.cpp
//  nightgarden
//
//  Created by Stephen Detwiler on 3/21/16.
//
//

#include "variable.h"

using namespace std;

Variable::Variable()
{
	value = std::numeric_limits<float>::quiet_NaN();
	type= Float;
}

Variable::Variable(Variable const& rhs)
{
	name = rhs.name;
	value = rhs.value;
	stringValue = rhs.stringValue;
	type = rhs.type;
}

Variable::~Variable()
{
	
}

std::string Variable::toString() const
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

Variable& Variable::operator=(Variable const& rhs)
{
	name = rhs.name;
	value = rhs.value;
	stringValue = rhs.stringValue;
	type = rhs.type;
	
	return *this;
}

Variable& Variable::operator=(float rhs)
{
	value = rhs;
	type = Variable::Float;
	
	return *this;
}

Variable& Variable::operator=(double rhs)
{
	value = rhs;
	type = Variable::Float;
	
	return *this;
}

Variable& Variable::operator=(int rhs)
{
	value = rhs;
	type = Variable::Float;
	
	return *this;
}

Variable& Variable::operator=(char const* rhs)
{
	stringValue = rhs;
	type = Variable::String;
	
	return *this;
}

Variable& Variable::operator=(std::string const& rhs)
{
	stringValue = rhs;
	type = Variable::String;
	
	return *this;
}


Variable Variable::operator+(Variable const& rhs)
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

Variable Variable::operator-(Variable const& rhs)
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

Variable Variable::operator*(Variable const& rhs)
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

Variable Variable::operator/(Variable const& rhs)
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

bool Variable::operator==(Variable const& rhs) const
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

bool Variable::operator!=(Variable const& rhs) const
{
	return !(*this == rhs);
}


bool  Variable::operator<(Variable const& rhs) const
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

bool  Variable::operator<=(Variable const& rhs) const
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

bool Variable::operator>(Variable const& rhs) const
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

bool  Variable::operator>=(Variable const& rhs) const
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



