#include "ParserMaterialSelector.h"
#include "WaveObject.h"

ParserMaterialSelector::ParserMaterialSelector(WaveObject* object)
{
	this->object = object;
}

void ParserMaterialSelector::parse(vector<string>& line)
{
	string materialName = line.at(1);

	object->currentMaterial = object->materials[materialName];
	object->currentGroup->material = object->currentMaterial;
}

