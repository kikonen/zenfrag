#pragma once

#include "includes.h"
#include "TextureLoader.h"
#include "Engine.h"


struct md5material
{
	string name;

	string mapFileName;
	unsigned int mapTextId;

	string diffuseMapFilename;
	unsigned int diffuseTextId;

	string specularMapFilename;
	unsigned int specularTextId;

	//Bump mapping
	string normalMapFilename;
	unsigned int normalTextId;

	string heightMapFilename;
	unsigned int heightTextId;

	string bumpFilename;
	unsigned int bumpTextId;

	

	md5material()
	{
		mapTextId=0;

		diffuseTextId=0;
		specularTextId=0;
		normalTextId=0;
		heightTextId=0;
		bumpTextId=0;
	}	

	void load()
	{
		if (mapFileName.size()			!=0){
			mapTextId		= TextureLoader::instance()->load((Engine::engine->modelFolder + mapFileName).c_str());
			if (mapTextId == 0)
				printf("Error loading map texture, shader name '%s'\n",name.c_str());
		}
		

		if (diffuseMapFilename.size()	!=0)
		{	
			diffuseTextId	= TextureLoader::instance()->load((Engine::engine->modelFolder + diffuseMapFilename).c_str());
			if (diffuseTextId == 0)
				printf("Error loading diffuse texture, shader name '%s'\n",name.c_str());
		}

		if (specularMapFilename.size()	!=0){
			specularTextId	= TextureLoader::instance()->load((Engine::engine->modelFolder + specularMapFilename).c_str());
			if (specularTextId == 0)
				printf("Error loading specular texture, shader name '%s'\n",name.c_str());

		}

		if (normalMapFilename.size()	!=0)
		{
			normalTextId	= TextureLoader::instance()->load((Engine::engine->modelFolder + normalMapFilename).c_str());
			if (normalTextId == 0)
				normalTextId	= TextureLoader::instance()->load((Engine::engine->modelFolder + normalMapFilename + ".tga").c_str());
			if (normalTextId == 0)
				printf("Error loading normal texture, shader name '%s'\n",name.c_str());
		}

		if (heightMapFilename.size()	!=0){
			heightTextId	= TextureLoader::instance()->load((Engine::engine->modelFolder + heightMapFilename).c_str());
			if (heightTextId == 0)
				heightTextId	= TextureLoader::instance()->load((Engine::engine->modelFolder + heightMapFilename + ".tga").c_str());
			if (heightTextId == 0)
				printf("Error loading heightMap texture, shader name '%s'\n",name.c_str());
		}

		if (bumpFilename.size()			!=0)
		{
			bumpTextId		= TextureLoader::instance()->load((Engine::engine->modelFolder + bumpFilename).c_str());
			if (bumpTextId == 0)
				printf("Error loading bumpMap texture, shader name '%s'\n",name.c_str());
		}
	}

	void show()
	{
		printf("Shader '%s':\n",name.c_str());
		printf("	map: id=%d - '%s'.\n",mapTextId,mapFileName.c_str());
		printf("	diffuse id=%d - '%s'.\n",diffuseTextId,diffuseMapFilename.c_str());
		printf("	specular id=%d - '%s'.\n",specularTextId,specularMapFilename.c_str());
		printf("	normal id=%d - '%s.\n",normalTextId,normalMapFilename.c_str());
		printf("	height id=%d - '%s'.\n",heightTextId,heightMapFilename.c_str());
		printf("	bump id=%d - '%s'.\n",bumpTextId,bumpFilename.c_str());

	}
};

class md5MaterialsLibrary
{
public:
	
	static md5MaterialsLibrary* getInstance();

	void load(const char* materialFile);
	md5material* getMaterial(const char* name);

private:
	bool debug;
	static md5MaterialsLibrary* instance ;
	int openedBracketCounter; // Used for material file parsing
	int lineCounter ;
	md5material* currentMaterial ; // Keep track of the state (current material)
	md5MaterialsLibrary();
	~md5MaterialsLibrary();

	map<string,md5material*> materials;

	void addContentToCurrentMaterial(string line);

	void md5MaterialsLibrary::tokenize(const string& str,vector<string>& tokens,const string& delimiters = " ");
	void analyseRenderBump(string& line);
};

