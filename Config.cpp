#include "Config.h"
#include "Engine.h"

#ifdef _WIN32
	#define CONFIG_FILE ".\\cfg\\config.txt"
#else
	#define CONFIG_FILE "./cfg/config.txt"
#endif



Config* Config::instance = NULL;
char Config::lineBuffer[1024]; //Reading 1024 bytes length lines...
char* Config::currentChar ;
string Config::token;
map<std::string,Config::FunctionPtr> Config::parsers;
 int Config::antiAliasingMSAA = 1;

Config::~Config(void)
{
}

Config* Config::getInstance()
{
	if (instance == NULL)
		instance = new Config();
	return instance;
}

void Config::load()
{
	string fileToOpen = string(CONFIG_FILE);
	
	FILE* fp = fopen (fileToOpen.c_str(), "rb");
	if (!fp)
    {
		printf("Error: could not open config file '%s'!\n", fileToOpen.c_str());
		return ;
    }
	
	

	while (fgets (lineBuffer, sizeof (lineBuffer), fp) != 0) 
	{
		currentChar = lineBuffer;
		//Remove \n from end of lineBuffer
		//lineBuffer[strlen(lineBuffer)-1] = '\0';
		token = string();
		skipWhiteSpace();
		readToken();
		//printf(" token = '%s'\n",token.c_str());
		callAppropriateParser();
		
	}
	
	fclose (fp);
}


void Config::skipWhiteSpace()
{
	// C style comment, skipping rest of the line
	

	while(*currentChar && (*currentChar == ' ' || *currentChar == '	' || *currentChar == '/'))
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

void Config::readToken()
{
	while(*currentChar && *currentChar != ' ' && *currentChar != '	' &&  *currentChar != '\r')
	{
		//printf("Adding character ='%d', value = '%c'\n",*currentChar,*currentChar);
		token+=  *currentChar;
		currentChar++;
	}
	//printf("Token read = ' %s ' \n",token.c_str());
}

void Config::readReal()
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
}

void Config::callAppropriateParser(void)
{
	FunctionPtr parser = parsers[token];
	token = string();
	if (parser != NULL)
		(*parser)();
}


void Config::parseRenderWidth(void)
{
	skipWhiteSpace();
	readReal();
	//printf(" render Width = '%.2f'\n",atof(token.c_str()));
	int renderWidth = atoi(token.c_str());
	if (renderWidth != 0)
		Engine::engine->renderWidth = renderWidth;
	else
		printf("Error in config, could not read renderWidth.\n");
}

void Config::parseRenderHeight(void)
{
	skipWhiteSpace();
	readReal();
	int renderHeight = atoi(token.c_str());
	if (renderHeight != 0)
		Engine::engine->renderHeight = renderHeight;
	else
		printf("Error in config, could not read renderHeight.\n");
}

void Config::parseFboLightScatRatio(void)
{
	skipWhiteSpace();
	readReal();
	float ratio = atof(token.c_str());
	if (ratio != 0.0)
		Engine::engine->fboLightScatteringRatio = ratio;
	else
		printf("Error in config, could not read FBO light scattering ratio.\n");
}
void Config::parseFboShadoBufRatio(void)
{
	skipWhiteSpace();
	readReal();
	float ratio = atof(token.c_str());
	if (ratio != 0.0)
		Engine::engine->fboShadowBuffersRatio = ratio;
	else
		printf("Error in config, could not read FBO shadow buffer ratio.\n");
}
void Config::parseLightScatActivated(void)
{

	skipWhiteSpace();
	readReal();
	int lightScatActive = atoi(token.c_str());
	Engine::engine->lightScatteringActivated = (lightScatActive != 0);
}
void Config::parseFullScreen(void)
{
	skipWhiteSpace();
	readReal();
	int fullScreen = atoi(token.c_str());
	Engine::engine->renderFullScreen = (fullScreen != 0);
}
void Config::parseShaderFolder(void)
{
	skipWhiteSpace();
	readToken();
	Engine::engine->shaderFolder = token;
	printf("folder path = '%s'\n",Engine::engine->shaderFolder.c_str());
}

void Config::parseModelFolder(void)
{
	skipWhiteSpace();
	readToken();
	Engine::engine->modelFolder = token;
	printf("model path = '%s'\n",Engine::engine->modelFolder.c_str());
}

void Config::parseAA(void)
{
	skipWhiteSpace();
	readReal();
	antiAliasingMSAA = atoi(token.c_str());
	printf("AA = %dx\n",antiAliasingMSAA);
}

Config::Config(void)
{
	parsers["renderWidth"] = &(parseRenderWidth);
	parsers["renderHeight"] = &(Config::parseRenderHeight);
	parsers["fboLightScatteringRatio"] = &(Config::parseFboLightScatRatio);
	parsers["fboShadowBuffersRatio"] = &(Config::parseFboShadoBufRatio);
	parsers["lightScatteringActivated"] = &(Config::parseLightScatActivated);
	parsers["renderFullScreen"] = &(Config::parseFullScreen);
	parsers["shaderFolder"] = &(Config::parseShaderFolder);
	parsers["modelFolder"] = &(Config::parseModelFolder);
	parsers["antiAliasing"] = &(Config::parseAA);
	

	loaded = false;
	load();
	loaded = !errorDuringLoading;

}