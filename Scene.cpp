#include "Scene.h"
#include "md5.h"
#include "md5MaterialsLibrary.h"
#include "Engine.h"
#include "CreatureShader.h"
#include "Surface.h"

Scene* Scene::instance = NULL;
char Scene::lineBuffer[1024]; //Reading 1024 bytes length lines...
char* Scene::currentChar ;
string Scene::token;
map<std::string,Scene::FunctionPtr> Scene::parsers;
Light*  Scene::currentLight ;
md5Object* Scene::currentObject;
Surface* Scene::currentSurface;

#ifdef _WIN32
	#define SCENE_FILE ".\\cfg\\scene.txt"
#else
	#define SCENE_FILE "./cfg/scene.txt"
#endif


Scene* Scene::getInstance()
{
	if (instance == NULL)
		instance = new Scene();
	return instance;
}




Scene::Scene(void)
{
	currentLight = NULL;

	parsers["light"] = &(Scene::parseLight);
	parsers["type"] = &(Scene::parseLightType);
	parsers["ambient"] = &(Scene::parseAmbient);
	parsers["diffuse"] = &(Scene::parseDiffuse);
	parsers["specular"] = &(Scene::parseSpecular);
	parsers["constantAttenuation"] = &(Scene::parseConsAtten);
	parsers["linearAttenuation"] = &(Scene::parseLinAtten);
	parsers["quadraticAttenuation"] = &(Scene::parseQuadAtten);
	parsers["lightPosition"] = &(Scene::parseLightPosition);
	parsers["lightLookAt"] = &(Scene::parseLightLookAt);
	parsers["material"] = &(Scene::parseMaterial);
	
	parsers["creature"] = &(Scene::parseCreature);
	parsers["Cposition"] = &(Scene::parseCreaturePosition);
	parsers["Crotation"] = &(Scene::parseCreatureRotation);
	parsers["wall"] = &(Scene::parseWall);
	parsers["floor"] = &(Scene::parseFloor);
	parsers["ceiling"] = &(Scene::parseCeiling);

	loaded = false;
	load();
	loaded = !errorDuringLoading;
}

Scene::~Scene(void)
{
}

void Scene::load(void)
{
		string fileToOpen = string(SCENE_FILE);
	
	FILE* fp = fopen (fileToOpen.c_str(), "rb");
	if (!fp)
    {
		printf("Error: could not open config file '%s'!\n", fileToOpen.c_str());
		return ;
    }
	
	

	while (fgets (lineBuffer, sizeof (lineBuffer), fp) != 0) 
	{
		currentChar = lineBuffer;
		token = string();
		skipWhiteSpace();
		readToken();
		callAppropriateParser();
		
	}
	
	fclose (fp);
}

void Scene::clearToken()
{
	token =string();
}

void Scene::skipWhiteSpace()
{
	// C style comment, skipping rest of the line
	

	while(*currentChar && (*currentChar == ' ' || *currentChar == '	' || *currentChar == '/' ||  *currentChar == '\r' || *currentChar == '\n'))
	{
		if (*currentChar == '/' )
		{	
			if (*(currentChar+1) == '/')
				currentChar = lineBuffer + strlen(lineBuffer);
			
			return;
		}
		currentChar++;
	}
}

void Scene::readToken()
{
	while(*currentChar && *currentChar != ' ' && *currentChar != '	' &&  *currentChar != '\r' && *currentChar != '\n')
	{
		//printf("Adding character ='%d', value = '%c'\n",*currentChar,*currentChar);
		token+=  *currentChar;
		currentChar++;
	}
	//printf("Token read = ' %s ' \n",token.c_str());
}

void Scene::readReal()
{
	while(
			*currentChar && 
			(
				(
					(*currentChar) - '0' >= 0 && 
					(*currentChar) <= '9'
				)		
				||   (*currentChar)== '.' 
				|| (*currentChar)== '-'
			)
		 
			)
	{
		token+=  *currentChar;
		currentChar++;
	}
	//printf(" readReal = '%s'\n",token.c_str());
}

void Scene::callAppropriateParser(void)
{
	FunctionPtr parser = parsers[token];
	clearToken();
	if (parser != NULL)
		(*parser)();
}


void Scene::parseLight(void)
{
	skipWhiteSpace();
	readReal();

	Light* light = new Light(Engine::engine->renderWidth,Engine::engine->renderHeight,Vertex(),Vertex(),1,1000);
	light->id = atoi(token.c_str());
	Engine::engine->lights.push_back(light);
	currentLight = light;
}

void Scene::parseLightType(void)
{
	//Ignored
}

void Scene::parseAmbient(void)
{
	for(int i=0 ; i < 4 ; i++)
	{
		skipWhiteSpace();
		readReal();
		currentLight->ambient[i] = atof(token.c_str());
		clearToken();
	}
}

void Scene::parseDiffuse(void)
{
	for(int i=0 ; i < 4 ; i++)
	{
		skipWhiteSpace();
		readReal();
		currentLight->diffuse[i] = atof(token.c_str());
		clearToken();
	}
}

void Scene::parseSpecular(void)
{
	for(int i=0 ; i < 4 ; i++)
	{
		skipWhiteSpace();
		readReal();
		currentLight->specular[i] = atof(token.c_str());
		clearToken();
	}
}

void Scene::parseConsAtten(void)
{
	skipWhiteSpace();
	readReal();
	currentLight->constantAttenuation = atof(token.c_str());
}

void Scene::parseLinAtten(void)
{
	skipWhiteSpace();
	readReal();
	currentLight->linearAttenuation = atof(token.c_str());
}

void Scene::parseQuadAtten(void)
{
	skipWhiteSpace();
	readReal();
	currentLight->quadraticAttenuation = atof(token.c_str());
}

void Scene::parseLightPosition(void)
{
	
	skipWhiteSpace();
	readReal();
	currentLight->position.x = atof(token.c_str());
	clearToken();

	skipWhiteSpace();
	readReal();
	currentLight->position.y = atof(token.c_str());
	clearToken();

	skipWhiteSpace();
	readReal();
	currentLight->position.z = atof(token.c_str());
	clearToken();

}

void Scene::parseLightLookAt(void)
{
	skipWhiteSpace();
	readReal();
	currentLight->lookAt.x = atof(token.c_str());
	clearToken();

	skipWhiteSpace();
	readReal();
	currentLight->lookAt.y = atof(token.c_str());
	clearToken();

	skipWhiteSpace();
	readReal();
	currentLight->lookAt.z = atof(token.c_str());
	clearToken();
}	

void Scene::parseMaterial()
{
	skipWhiteSpace();
	readToken();
	md5MaterialsLibrary::getInstance()->load(token.c_str());
}

void Scene::parseWall(void)
{
	currentSurface = new Surface(Surface::WALL);
	//printf("WALL\n");
	parseSurface();
}
void Scene::parseFloor(void)
{
	currentSurface = new Surface(Surface::FLOOR);
	//printf("FLOOR\n");
	parseSurface();
}

void Scene::parseCeiling(void)
{
	currentSurface = new Surface(Surface::CEILING);
	//printf("CEILING\n");
	parseSurface();
}

void Scene::parseSurface(void)
{
	
	
	while (*currentChar)
	{
		clearToken();
		skipWhiteSpace();
		readToken();

		if (!token.compare("-min"))
		{ 
			clearToken();
			skipWhiteSpace();
			readReal();
			currentSurface->min.x = atof(token.c_str());
			//printf("wall min.x=%.2f\n",currentSurface->min.x);

			clearToken();
			skipWhiteSpace();
			readReal();
			currentSurface->min.y = atof(token.c_str());
			//printf("wall min.y=%.2f\n",currentSurface->min.y);

			clearToken();
			skipWhiteSpace();
			readReal();
			currentSurface->min.z = atof(token.c_str());
			//printf("wall min.z=%.2f\n",currentSurface->min.z);

			continue;
		}

		if (!token.compare("-max"))
		{ 
			clearToken();
			skipWhiteSpace();
			readReal();
			currentSurface->max.x = atof(token.c_str());
			//printf("wall max.x=%.2f\n",currentSurface->max.x);

			clearToken();
			skipWhiteSpace();
			readReal();
			currentSurface->max.y = atof(token.c_str());
			//printf("wall max.y=%.2f\n",currentSurface->max.y);

			clearToken();
			skipWhiteSpace();
			readReal();
			currentSurface->max.z = atof(token.c_str());
			//printf("wall max.z=%.2f\n",currentSurface->max.z);

			continue;
		}

		if (!token.compare("-densityX"))
		{ 
			clearToken();
			skipWhiteSpace();
			readReal();
			currentSurface->densityx = atoi(token.c_str());
			//printf("densityx = %d\n",currentSurface->densityx);
			continue;
		}

		if (!token.compare("-densityY"))
		{ 
			clearToken();
			skipWhiteSpace();
			readReal();
			currentSurface->densityy = atoi(token.c_str());
			//printf("densityy = %d\n",currentSurface->densityy);
			continue;
		}

		if (!token.compare("-material"))
		{ 
			clearToken();
			skipWhiteSpace();
			readToken();
			//printf("material = '%s'\n",token.c_str());
			currentSurface->material = md5MaterialsLibrary::getInstance()->getMaterial(token.c_str());
			if (currentSurface->material == NULL)
				printf("Error could not load material '%s'.\n",token.c_str());
			continue;
		}

		if (!token.compare("-normal"))
		{
			clearToken();
			skipWhiteSpace();
			readReal();
			currentSurface->normal.x = atof(token.c_str());
			//printf("wall normal.x=%.2f\n",currentSurface->normal.x);

			clearToken();
			skipWhiteSpace();
			readReal();
			currentSurface->normal.y = atof(token.c_str());
			//printf("wall normal.y=%.2f\n",currentSurface->normal.y);

			clearToken();
			skipWhiteSpace();
			readReal();
			currentSurface->normal.z = atof(token.c_str());
			//printf("wall normal.z=%.2f\n",currentSurface->normal.z);
		}
	}

	//wall.pack();	// Generate normal and tangent
	Engine::engine->surfaces.push_back(currentSurface);
}

void Scene::parseCreature(void)
{
	string model;
	string anim;
	int noDraw =-1;
	do
	{
		clearToken();
		skipWhiteSpace();
		readToken();
		string parameter = token;
		//printf(" parameter = '%s'\n",token.c_str());

		clearToken();
		skipWhiteSpace();
		readToken();
		string value = token;
		//printf(" value = '%s'\n",token.c_str());

		if (!parameter.compare("-model"))
			model = value;

		if (!parameter.compare("-anim"))
			anim = value;

		if (!parameter.compare("-nodraw"))
			noDraw = atoi(value.c_str());
	}
	while(token.size() != 0);

	md5Object* newCreature = new md5Object();
	//printf("creat creat with '%s', '%s'",model.c_str(),anim.c_str());
	newCreature->set(model.c_str(),anim.c_str());
	newCreature->noDraw = noDraw;
	currentObject = newCreature;
	Engine::engine->creatures.push_back(newCreature);
}

void Scene::parseCreaturePosition(void)
{
	skipWhiteSpace();
	readReal();
	currentObject->position.x = atof(token.c_str());
	clearToken();

	skipWhiteSpace();
	readReal();
	currentObject->position.y = atof(token.c_str());
	clearToken();

	skipWhiteSpace();
	readReal();
	currentObject->position.z = atof(token.c_str());
	clearToken();

}

void Scene::parseCreatureRotation(void)
{
	skipWhiteSpace();
	readReal();
	currentObject->orientation = atof(token.c_str());
	clearToken();

}