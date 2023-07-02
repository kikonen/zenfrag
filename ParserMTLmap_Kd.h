#pragma once

#include "Parser.h"
#include <vector>

class ParserMTLmap_Kd : public Parser
{
	public:

	ParserMTLmap_Kd(WaveObject* object);
	virtual  void parse(vector<string>& line);
	
	

};