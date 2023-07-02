#include "ParserMTLNewMaterial.h"
#include "WaveObject.h"

ParserMTLNewMaterial::ParserMTLNewMaterial(WaveObject* object)
{
	this->object = object;
}

void ParserMTLNewMaterial::parse(vector<string>& line)
{
	string materialName = line.at(1);

	Material* material = new Material();
	material->name = materialName;

	object->currentMaterial = material;
	object->materials[materialName] = material;
}
