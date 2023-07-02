#include "ParserMTLKa.h"
#include "WaveObject.h"

ParserMTLKa::ParserMTLKa(WaveObject* object)
{
	this->object = object;
}

void ParserMTLKa::parse(vector<string>& line)
{
	static float r;
	static float g;
	static float b;
	sscanf(line.at(1).c_str(), "%f", &r);
	sscanf(line.at(2).c_str(), "%f", &g);
	sscanf(line.at(3).c_str(), "%f", &b);

	Color* color = new Color(r,g,b,1.0f);

	object->currentMaterial->ka = color;
}