#pragma once

#include "Parser.h"
#include <vector>

class ParserVertice : public Parser
{
	public:

	ParserVertice(WaveObject* object);
	virtual  void parse(vector<string>& line);


};