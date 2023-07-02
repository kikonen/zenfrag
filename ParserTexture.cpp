#include "ParserTexture.h"
#include "WaveObject.h"

ParserTexture::ParserTexture(WaveObject* object)
{
	this->object = object;
}

void ParserTexture::parse(vector<string>& line)
{
	static float u;
	static float v;

	sscanf(line.at(1).c_str(), "%f", &u);
	sscanf(line.at(2).c_str(), "%f", &v);
	
	//printf("x=%f,y=%f,z=%f\n",x,y,z);
	this->object->textCoords.push_back(new TextCoord(u,1- v));
}
