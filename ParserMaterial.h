#pragma once

#include "Parser.h"
#include <vector>
#include <map>

class ParserMaterial : public Parser
{
	public:

	ParserMaterial(WaveObject* object);
	virtual  void parse(vector<string>& line);

private:
	map<string,Parser*> parsers;
};