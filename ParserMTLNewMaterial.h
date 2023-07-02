#pragma once

#include "Parser.h"
#include <vector>

class ParserMTLNewMaterial : public Parser
{
	public:

	ParserMTLNewMaterial(WaveObject* object);
	virtual  void parse(vector<string>& line);
	
	

};