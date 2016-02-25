/*
 * PEinfo.cpp
 *
 *  Created on: October 22, 2014
 *  Author: Moustafa
 *  Version: 1.0
 *
 */

#ifndef _ARGPARSER_
#define _ARGPARSER_


#include <iostream>
#include <fstream>
#include "Typedef.h"

using namespace std;



class ArgParser
{
    int argc;
    char** argv;
    
public:
	
	ArgParser();
	ArgParser(int argcount, char* argvs[]);
	~ArgParser();
    
	bool isValidPath(string path);
    bool checkArgs(int argc, char* argv[], char* arg);
    int getFileArgs(int argc, char* argv[]);
	//bool parseArgs(int argc, char* argv[]);
};



#endif
