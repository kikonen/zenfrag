#pragma once

#include "Parser.h"
#include <vector>

class ParserNormal : public Parser
{
	public:

	ParserNormal(WaveObject* object);
	virtual  void parse(vector<string>& line);


};