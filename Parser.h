#pragma once

class WaveObject;

//#include "WaveObject.h"

#include <string>
#include <vector>
using namespace std;
#include <sstream>
#include <iostream>
#include "3DDefinitions.h"

class Parser
{
public:

	virtual  void parse(vector<string>& line) = 0;

protected:
	WaveObject* object;
	void tokenize(const string& str,vector<string>& tokens,const string& delimiters );
};
