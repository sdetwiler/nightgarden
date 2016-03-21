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
	symbolList = nullptr;
}

Result::Result(Result const& rhs)
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

Result::~Result()
{
	if(symbolList)
	{
		delete symbolList;
	}
}

std::string Result::toString() const
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

std::string Result::toTimedString() const
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


