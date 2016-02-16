//
//  main.cpp
//  parser
//
//  Created by Stephen Detwiler on 2/13/16.
//  Copyright © 2016 Pirate Studios. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "lsystem.h"


int main(int argc, const char * argv[]) {
	std::cout << "Hello, World!\n";
	
	
//	char const* testString = "A->BCDEFG\nB->XYZ\n";

	// TODO: FIXME
	char const* filename = "/Users/steve/projects/nightgarden/data/test.ls";
	std::ifstream infile(filename);
	if(!infile.is_open())
	{
		std::cout << "Failed to open " << filename << std::endl;
		return -1;
	}
	std::string data;
	std::string line;
	while(std::getline(infile, line))
	{
		data+=line;
		data+="\n";
	}
	
	if(infile.bad())
	{
		std::cout << "Error while reading from " << filename << std::endl;
		return -1;
	}
		
	LSystem& lsystem = LSystem::getInstance();
	if(lsystem.parse(data.c_str()))
	{
		lsystem.dumpVariables();
		lsystem.dumpRules();
		
		
		
		SymbolList const* state;
		
		state = lsystem.getState();
		std::cout << state->toString() << std::endl;
		
		
		for(int i=0; i<4; ++i)
		{
			lsystem.step();
			state = lsystem.getState();
			
			std::cout << state->toString() << std::endl;
		}
		
		
		lsystem.clear();
	}
	
	
    return 0;
}
