#include "ParserMTLKd.h"
#include "WaveObject.h"

ParserMTLKd::ParserMTLKd(WaveObject* object)
{
	this->object = object;
}

void ParserMTLKd::parse(vector<string>& line)
{
		static float r;
	static float g;
	static float b;
	sscanf(line.at(1).c_str(), "%f", &r);
	sscanf(line.at(2).c_str(), "%f", &g);
	sscanf(line.at(3).c_str(), "%f", &b);

	Color* color = new Color(r,g,b,1.0f);

	object->currentMaterial->kd = color;
}