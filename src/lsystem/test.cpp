//
//  test.cpp
//  parser
//
//  Created by Stephen Detwiler on 2/13/16.
//  Copyright © 2016 Pirate Studios. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "lsystem.h"

using namespace std;

int parser_test()
{
	cout << "parser_test: Hello, World!\n";
	
	
//	char const* testString = "A->BCDEFG\nB->XYZ\n";

	// TODO: FIXME
	char const* filename = "/Users/steve/projects/nightgarden/data/test.ls";
	ifstream infile(filename);
	if(!infile.is_open())
	{
		cout << "Failed to open " << filename << endl;
		return -1;
	}
	string data;
	string line;
	while(getline(infile, line))
	{
		data+=line;
		data+="\n";
	}
	
	if(infile.bad())
	{
		cout << "Error while reading from " << filename << endl;
		return -1;
	}
	// FIXME
	LSystem& lsystem = LSystem::getInstance();
	if(lsystem.parse(data.c_str()))
	{
		lsystem.dumpVariables();
		lsystem.dumpRules();
		
		
		
		SymbolList const* state;
		
		state = lsystem.getState();
		if(state)
		{
			cout << "Intial State" <<  endl;
			cout << state->toString() << endl << endl;
		}
		
		
		for(int i=0; i<4; ++i)
		{
			lsystem.step(1.0);
			state = lsystem.getState();
			if(state)
			{
//				cout << "Curr State" << endl;
//				cout << state->toString() << endl;
			}
		}
		
		cout << "Final State" << endl;
		cout << state->toString() << endl;
		
		//lsystem.clear();
	}
	else
	{
		cout << "Failed to parse lsystem from file." << endl;
	}
	
	
    return 0;
}
