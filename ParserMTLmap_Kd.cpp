#include "ParserMTLmap_Kd.h"
#include "WaveObject.h"
#include "TextureLoader.h"

ParserMTLmap_Kd::ParserMTLmap_Kd(WaveObject* object)
{
	this->object = object;
}

void ParserMTLmap_Kd::parse(vector<string>& line)
{
	string textureName = line.at(1);
	string fileToOpen = WaveObject::pathContext + textureName;
	int textureId = TextureLoader::instance()->load(fileToOpen.c_str());

	object->currentMaterial->textureId = textureId;
	object->currentMaterial->textureFileName = textureName;
}