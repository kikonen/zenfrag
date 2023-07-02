#include "ParserNormal.h"
#include "WaveObject.h"

ParserNormal::ParserNormal(WaveObject* object)
{
	this->object = object;
}

void ParserNormal::parse(vector<string>& line)
{
	static float x;
	static float y;
	static float z;
	sscanf(line.at(1).c_str(), "%f", &x);
	sscanf(line.at(2).c_str(), "%f", &y);
	sscanf(line.at(3).c_str(), "%f", &z);
	
	//printf("x=%f,y=%f,z=%f\n",x,y,z);
	this->object->normals.push_back(new Vertex(x,y,z));
}

