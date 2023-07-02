#pragma once

#include "Parser.h"
#include <vector>

class ParserMTLKs : public Parser
{
	public:

	ParserMTLKs(WaveObject* object);
	virtual  void parse(vector<string>& line);
	
	

};