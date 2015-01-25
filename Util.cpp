/*
 * Util.cpp
 *
 *  Created on: January 4, 2015
 *  Author: Moustafa
 *  Version: 1.0
 *
 * This file has some utiliti functions
 */
 
#include "Util.h"
#include <string>
#include <sstream>
#include <iostream>
#include <string.h>
#include <stdio.h>

using namespace std;

string int2HexStr(int n)
{
	std::stringstream ss;
	string s;
	ss << std::hex << n;
	ss >> s;
	return s;
}


string getFileName(string path)
{
	int i = path.length()-1;	// point to last char
	string FileName;

	while(path[i] != PATHSEPARATOR && i > 0)
	{
		i--;
	};
	if(i > 0)	FileName = path.substr(i+1);
	else		FileName = path;
	return FileName;
}

// untested, check the other getLineFromMem() which is tested.
DWORD getLineFromMem(LPVOID ReadAddr, LPVOID Bound, char* &Line)
{
	DWORD LimitSize = (DWORD)Bound - (DWORD)ReadAddr;
	BYTE x = *(BYTE *)ReadAddr;
	DWORD i = 0, StrLen = 0;

	while( (x == '\r' || x == '\n' || x == '\0' || x == EOF || x < 0x20 || x > 0x7E) && (i < LimitSize))
		x = *((BYTE *)ReadAddr + ++i);

	for(; i < LimitSize; i++)
	{
		if(x == '\r' || x == '\n' || x == '\0' || x == EOF)
			break;
		
		StrLen++;
		x = *((BYTE *)ReadAddr + i + 1);
	}

	// if end of file
	if(x == EOF || x == 0)	
		ReadAddr = Bound;
	Line =  new char[StrLen+1];
	memcpy(Line, (LPVOID)((DWORD)ReadAddr + i - StrLen), StrLen);
	Line[StrLen] = '\0';
	return i;
}

string getLineFromMem(LPVOID &ReadAddr, LPVOID Bound)
{
	int LimitSize = (DWORD)Bound - (DWORD)ReadAddr;

	BYTE x = *(BYTE *)ReadAddr;
	int i = 1, StrLen = 0;
	char* Line;

	// skip newlines and non printable characters
	while( (x == '\r' || x == '\n' || x == '\0' || x == EOF || x < 0x20 || x > 0x7E) && (i < LimitSize))
		x = *((BYTE *)ReadAddr + i++);

	for(; i < LimitSize; i++)
	{
		// I allow character out of the range 0x20-0x7E here for unicode tool names
		if(x == '\r' || x == '\n' || x == '\0' || x == EOF)
			break;
		
		StrLen++;
		x = *((BYTE *)ReadAddr + i);
	}
	// if end of file
	if(x == EOF || x == 0)	
		ReadAddr = Bound;
	ReadAddr = (LPVOID) ((DWORD) ReadAddr + i);				// update memory pointer
	if(StrLen) {
		Line =  new char[StrLen+1];
		memcpy(Line, (LPVOID)((DWORD)ReadAddr - 1 - StrLen), StrLen);
		Line[StrLen] = '\0';
		string s = string(Line);
		delete Line;
		return s;
	}
	
	return "";
}

string removeSpaces(string s)
{
	string s2;
	for(int i = 0; i < s.length(); i++)
		if(!isspace(s[i]))	s2.append(1, s[i]);
	return s2;
}