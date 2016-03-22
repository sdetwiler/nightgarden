//
//  result.h
//  nightgarden
//
//  Created by Stephen Detwiler on 3/21/16.
//
//

#ifndef result_h
#define result_h

#include <string>
#include "symbol.h"


////////////////////////////////////////////////////////////////////////////////
class Result
{
public:
	SymbolList symbolList;
	
	Result();
	Result(Result const& rhs);
	~Result();
	std::string toString() const;
	std::string toTimedString() const;
};


#endif /* result_h */
