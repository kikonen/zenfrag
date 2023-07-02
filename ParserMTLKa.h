#pragma once

#include "Parser.h"
#include <vector>

class ParserMTLKa : public Parser
{
	public:

	ParserMTLKa(WaveObject* object);
	virtual  void parse(vector<string>& line);
	
	

};