#pragma once

#include "Parser.h"
#include <vector>

class ParserMaterialSelector : public Parser
{
	public:

	ParserMaterialSelector(WaveObject* object);
	virtual  void parse(vector<string>& line);


};