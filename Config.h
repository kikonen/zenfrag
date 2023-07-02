#pragma once

#include "includes.h"

class Config
{
public:
	
	~Config(void);
	static Config* getInstance();
	void load();
	bool loaded;

	static int antiAliasingMSAA;

private:
	static Config* instance;
	Config(void);

	bool errorDuringLoading;
	
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
	
	static void parseRenderWidth(void);
	static void parseRenderHeight(void);
	static void parseFboLightScatRatio(void);
	static void parseFboShadoBufRatio(void);
	static void parseLightScatActivated(void);
	static void parseFullScreen(void);
	static void parseShaderFolder(void);
	static void parseModelFolder(void);
	static void parseAA(void);

	
};
