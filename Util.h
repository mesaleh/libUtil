/*
 * Util.h
 *
 *  Created on: January 4, 2015
 *  Author: Moustafa
 *  Version: 1.0
 *
 * This file has some utiliti functions
 */

#ifndef _UTIL_
#define _UTIL_

#ifndef __linux__
	#define PATHSEPARATOR '\\'
	#include <windows.h>
#else
	#define PATHSEPARATOR '/'
	#include "Typedef.h"
#endif

using namespace std;

#include <string>
#include <sstream>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>


string int2HexStr(int n);

string getFileName(string path);

DWORD getLineFromMem(LPVOID LoadAddr, LPVOID Bound, char* &Line);

string getLineFromMem(LPVOID &ReadAddr, LPVOID Bound);

// trim from start
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}

template <typename T>
string numToStr(T number)
{
	string s = dynamic_cast<stringstream *> (&(stringstream() << std::uppercase << number))->str();
	return (s);
}

string removeSpaces(string s);

inline int hexStr2int(string s)
{
	//return std::stoul(s, nullptr, 16);

	int x;   
	std::stringstream ss;
	ss << std::hex << s;
	ss >> x;
	return x;
}


#endif