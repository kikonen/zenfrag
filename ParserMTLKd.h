#pragma once

#include "Parser.h"
#include <vector>

class ParserMTLKd : public Parser
{
	public:

	ParserMTLKd(WaveObject* object);
	virtual  void parse(vector<string>& line);
	
	

};