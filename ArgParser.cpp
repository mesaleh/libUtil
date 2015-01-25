/*
 * ArgParser.cpp
 *
 *  Created on: October 22, 2014
 *  Author: Moustafa
 *  Version: 1.0
 *
 */


#include "ArgParser.h"
#include <cstring>
#include <fstream>

ArgParser::ArgParser()
{
}



ArgParser::~ArgParser()
{
}

bool ArgParser::isValidPath(string path)
{
	ifstream testF(path.c_str());
	bool isGood = false;
	if(testF.good())	isGood = true;
	testF.close();

	return isGood;
}

bool ArgParser::checkArgs(int argc, char* argv[], char* arg)
{
	// assuming format of the command line is "[program name] [-arg1] [-arg2] .. [-arg3] [file(s)]
	int i = 1;
	while(i < argc && argv[i][0] == '-')
	{
		if(!strcmp(argv[i], arg))	return true;
		i++;
	}
	return false;
}

int ArgParser::getFileArgs(int argc, char* argv[])
{
	// assuming format of the command line is "[program name] [-arg1] [-arg2] .. [-arg3] [file(s)]
	int i = 1;
	while(i < argc && argv[i][0] == '-')	i++;

	if(i < argc)	return i;
	return 0;
}

