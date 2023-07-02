#pragma once

#include "Parser.h"
#include <vector>

class ParserTexture : public Parser
{
	public:

	ParserTexture(WaveObject* object);
	virtual  void parse(vector<string>& line);


};