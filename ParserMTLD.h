#pragma once

#include "Parser.h"
#include <vector>

class ParserMTLD : public Parser
{
	public:

	ParserMTLD(WaveObject* object);
	virtual  void parse(vector<string>& line);
	
	

};