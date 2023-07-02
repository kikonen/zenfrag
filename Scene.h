#pragma once

#include "includes.h"
#include "Light.h"
#include "md5.h"

class Scene
{
public:
	
	~Scene(void);
	static Scene* getInstance();
	void load();
	bool loaded;

private:
	static Scene* instance;
	Scene(void);

	bool errorDuringLoading;
	
	static Light* currentLight ;
	static md5Object* currentObject;
	static Surface* currentSurface;

	//typedef point2funct 
	typedef void (*FunctionPtr)(void );
	static map<std::string,FunctionPtr> parsers;		// Parsers is a map of function pointers
	//Parsing variables
	static char lineBuffer[1024]; //Reading 512 bytes length lines...
	static char* currentChar ;
	static string token;


		//Parsing methods
	static void skipWhiteSpace();
	static void readToken();
	static void readReal();
	static void callAppropriateParser();
	static void clearToken();


	static void parseLight(void);
	static void parseLightType(void);
	static void parseAmbient(void);
	static void parseDiffuse(void);
	static void parseSpecular(void);
	static void parseConsAtten(void);
	static void parseLinAtten(void);
	static void parseQuadAtten(void);
	static void parseLightPosition(void);
	static void parseLightLookAt(void);
	static void parseMaterial(void);
	static void parseCreature(void);
	static void parseCreaturePosition(void);
	static void parseCreatureRotation(void);
	static void parseWall(void);
	static void parseFloor(void);
	static void parseCeiling(void);
	static void parseSurface(void);
};
