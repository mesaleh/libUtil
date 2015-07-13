/*
 * Util.cpp
 *
 *  Created on: January 4, 2015
 *  Author: Moustafa
 *  Version: 1.0
 *
 * This file has some utility functions
 */

#include <regex>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string.h>
#include <stdio.h>
#ifdef __linux__
#include <sys/stat.h>
#include <sys/resource.h>
#endif
#include "Util.h"

using namespace std;


// private functions
#ifndef __linux__
DWORD _getFileAttributes(char* path)
{
	wchar_t* wstr = new wchar_t[strlen(path)+1];
    int len = std::mbstowcs(wstr, path, MAX_PATH);
	DWORD res = INVALID_FILE_ATTRIBUTES;
	if(len > 0)	{
		wstr[len] = NULL;
		res = GetFileAttributesW((LPCWSTR)wstr);
	}
	delete[] wstr;
	return res;
}
#endif

// public functions

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
		delete[] Line;
		return s;
	}
	
	return "";
}

/*
* The functions escape the following:
- it adds a single backslash to an odd number of backslashes.
- removes non-printable characters. (should we remove or replace it by another one for unescape())
- replace " with \"
*/
string escapeForJson(string s)
{
	// The order of replacement is important!
	std::regex e2("(^|[^\\\\])(\\\\(\\\\\\\\)*)(?!\\\\)");		// if a sequence of backslashes was of odd length, add a backslashes to make it even
	std::regex e3("[^[:print:]]");								// remove all non-printable character, equivalent to str.encoding('ascii', 'ignore') in python
	std::regex e4("\\\"");										// replace a double quote with a backslash double quote
	s = std::regex_replace(s, e2, "$1$2\\");
	s = std::regex_replace(s, e3, "");
	s = std::regex_replace(s, e4, "\\\"");
	return s;
}

/* remove spaces in between characters */
string removeSpaces(string s)
{
	string s2;
	for(int i = 0; i < s.length(); i++)
		if(!isspace(s[i]))	s2.append(1, s[i]);
	return s2;
}

#ifdef __linux__

bool isFile(char* path) {
    struct stat buf;
    stat(path, &buf);
    return S_ISREG(buf.st_mode);
}

bool isDir(char* path) {
    struct stat buf;
    stat(path, &buf);
    return S_ISDIR(buf.st_mode);
}

bool isFileExists(char* file) {
  struct stat buffer;   
  return (stat (file, &buffer) == 0); 
}

int increaseStackSize(unsigned int Size)
{
	rlim_t kStackSize = Size;   
    struct rlimit rl;
    int result;
    result = getrlimit(RLIMIT_STACK, &rl);
    if (result == 0)
    {
        if (rl.rlim_cur < kStackSize)
        {
            rl.rlim_cur = kStackSize;
            result = setrlimit(RLIMIT_STACK, &rl);
            if (result != 0)
            {
                fprintf(stderr, "setrlimit returned result = %d\n", result);
            }
        }
    }
	return result;
}
#else

bool isFile(char* path) {
	DWORD res = _getFileAttributes(path);
	return !(res & FILE_ATTRIBUTE_DIRECTORY);
}

bool isDir(char* path) {	
	DWORD res = _getFileAttributes(path);
	return (res & FILE_ATTRIBUTE_DIRECTORY);
}

// Checks if file exists. This is a faster implementation than fstream way of C++
bool isFileExists(char* path)
{
	DWORD res = _getFileAttributes(path);
	return (res != INVALID_FILE_ATTRIBUTES && !(res & FILE_ATTRIBUTE_DIRECTORY));
}
#endif

bool isValidPath(string x)
{

	// file name cannot contain any of < > : " / \ | ? *, to match them use std::regex r1("[<>:\"/\\\\\\|\\*\\?]");
	// However, the path can have :, ?, \, / as in c:\, or \\?\.
	std::regex r1("[<>\"\\|\\*]");				// does the string contain any of < > " | *
	std::regex r2("[^[:print:]]");				// does it contain non-printable chars
	std::regex r3("(.*[ ]+)");					// ends with a space(s)

	return !(std::regex_search(x, r1) || std::regex_search(x, r2) || std::regex_match(x, r3));

	// more checks to be added
}
