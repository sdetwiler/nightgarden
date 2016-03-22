//
//  result.cpp
//  nightgarden
//
//  Created by Stephen Detwiler on 3/21/16.
//
//

#include "result.h"

#include "types.h"

Result::Result()
{
}

Result::Result(Result const& rhs)
{
	symbolList = rhs.symbolList;
}

Result::~Result()
{
}

std::string Result::toString() const
{
	return symbolList.toString();
}

std::string Result::toTimedString() const
{
	return symbolList.toTimedString();
}


