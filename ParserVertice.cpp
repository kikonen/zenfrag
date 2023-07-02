#include "ParserVertice.h"
#include "WaveObject.h"

ParserVertice::ParserVertice(WaveObject* object)
{
	this->object = object;
}

void ParserVertice::parse(vector<string>& line)
{
	static float x;
	static float y;
	static float z;
	sscanf(line.at(1).c_str(), "%f", &x);
	sscanf(line.at(2).c_str(), "%f", &y);
	sscanf(line.at(3).c_str(), "%f", &z);
	
	//printf("x=%f,y=%f,z=%f\n",x,y,z);
	this->object->vertices.push_back(new Vertex(this->object->offSet.x+ x*this->object->scaleFactor,
												this->object->offSet.y+ y*this->object->scaleFactor,
												this->object->offSet.z+ z*this->object->scaleFactor));
}

