#pragma once

#include "Parser.h"
#include <vector>

class ParserFace : public Parser
{
	public:

	ParserFace(WaveObject* object);
	virtual  void parse(vector<string>& line);
	
	

};