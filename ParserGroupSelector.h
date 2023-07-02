#pragma once

#include "Parser.h"
#include <vector>

class ParserGroupSelector : public Parser
{
	public:

	ParserGroupSelector(WaveObject* object);
	virtual  void parse(vector<string>& line);


};