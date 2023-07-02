#include "md5MaterialsLibrary.h"
#include "includes.h"
#include "Lexer.h"

#define DEBUG 0

md5MaterialsLibrary::md5MaterialsLibrary()
{
	openedBracketCounter =0;
	lineCounter = 0;
	currentMaterial = NULL;

	debug = false;
	
}
md5MaterialsLibrary::~md5MaterialsLibrary(){}

md5MaterialsLibrary* md5MaterialsLibrary::instance = NULL;

md5MaterialsLibrary* md5MaterialsLibrary::getInstance()
{
	if (instance == NULL)
		instance = new md5MaterialsLibrary();
	return instance;
}

// This method is state based, It monitor token "{" and "}" to track beginning of shader declaration.
// If a "{" is encountred and openedBracketCounter==0 (the latter test is performed in order to avoid detecting a shader in nested {} blocks)
// then a new shader is created. 

void md5MaterialsLibrary::load(const char* materialFile)
{
   ifstream file;
   string absolutePath = Engine::engine->modelFolder ;
   absolutePath += materialFile;
   file.open(absolutePath.c_str());

   if (!file)
	{
		printf("Could not load Material file:'%s'\n",materialFile);
		return;
	}

   //printf("Material file opened: '%s'\n",absolutePath.c_str());

   string line;
   string previousLine; // Keeping track of previous line for material detection
   
   while (getline(file, line) )
   {
	   lineCounter++;
	  // if (DEBUG)
	//		printf("MD5 Material Parser:------ '%s'\n",materialFile);

	   size_t tableDeclaration = line.find("table ",0);
	   if (tableDeclaration == 0)
	   {
			if (DEBUG)
				printf("MD5 Material Parser:Found table(skipping)line#%d: '%s'\n",lineCounter,materialFile);
			previousLine = string(line);
			continue;
	   }

	   size_t openingBracket = line.find("{",0);
	   if (openingBracket != string::npos)
	   {
		   openedBracketCounter++;

		   if (openedBracketCounter == 1) // The previous line was the declaration of a new material
		   {
			   // Creating the material
			   md5material* newMaterial = new md5material();
			   newMaterial->name = previousLine;
			   currentMaterial = newMaterial;

				if (DEBUG)
					printf("Found new material: ln#:%d :'%s'\n",lineCounter-1,newMaterial->name.c_str());

			   // Adding material to the library
			   this->materials[newMaterial->name] = newMaterial;
		   }
	   }
	   
	    size_t closingBracket = line.find("}",0);
		if (closingBracket != string::npos)
			openedBracketCounter--;

		if (closingBracket == string::npos && openingBracket == string::npos && currentMaterial != NULL)
			addContentToCurrentMaterial(line);
		
		// Detect comments between shader name declaration and opening bracket
		bool lineIsAComment = false;
		size_t commentTagLocation = line.find("// ",0);
		if (commentTagLocation == 0)
			lineIsAComment = true;
		

		if (line.size() > 0 && !lineIsAComment) // There is sometimes a space between the name of the shader and the opening braket
			previousLine = string(line);
   }

	  
   file.close();   
   
}

void md5MaterialsLibrary::addContentToCurrentMaterial(string line)
{
	// Using a lexer 
	// Need to grab:
	//			- map
	//			- diffusemap
	//			- specularmap
	//			- bumpmap
	//			diffult declaration such as blend with follwing diffusemap,specularmap,bumpmap
	// renderbump not preceded with map are not used as info is redondant with bumpmap

	Lexer lexer = Lexer(line.c_str());
	
	if (debug)
		printf("\n\nLexer just got '%s'\n",line.c_str());

	while(!lexer.done())
	{
		lexer.clearToken();
		lexer.skipWhiteSpace();
		lexer.readToken();

		if (debug)
		printf("token %s \n",lexer.token.c_str());


		if ("map" == lexer.token)
		{
			lexer.clearToken();
			lexer.skipWhiteSpace();
			lexer.readToken();
			currentMaterial->mapFileName = lexer.token;
			if (debug)
				printf("map = '%s' \n",lexer.token.c_str());
			lexer.clearToken();
			continue;
		}

		if ("specularmap" == lexer.token)
		{
			lexer.clearToken();
			lexer.skipWhiteSpace();
			lexer.readToken();
			currentMaterial->specularMapFilename = lexer.token;
			if (debug)
				printf("specularmap = '%s' \n",currentMaterial->specularMapFilename.c_str());
			lexer.clearToken();
			continue;
		}

		if ("diffusemap" == lexer.token)
		{
			lexer.clearToken();
			lexer.skipWhiteSpace();
			lexer.readToken();
			currentMaterial->diffuseMapFilename = lexer.token;
			if (debug)
				printf("diffuseMapFilename = '%s' \n",currentMaterial->diffuseMapFilename.c_str());
			lexer.clearToken();
			continue;
		}

		if ("bumpmap" == lexer.token)
		{
			lexer.clearToken();
			lexer.skipWhiteSpace();
			lexer.readToken();

			if ("addnormals" == lexer.token)
			{
				lexer.clearToken();
				lexer.skipWhiteSpace();
				lexer.readToken();
				currentMaterial->bumpFilename = lexer.token;
				if (debug)
				printf("bumpFilename = '%s' \n",currentMaterial->bumpFilename.c_str() );

				//Skipping the "heightmap keyword
				lexer.clearToken();
				lexer.skipWhiteSpace();
				lexer.readToken();

				lexer.clearToken();
				lexer.skipWhiteSpace();
				lexer.readToken();
				currentMaterial->heightMapFilename = lexer.token;
				if (debug)
				printf("heightMapFilename = '%s' \n",currentMaterial->heightMapFilename.c_str() );
			}
			lexer.clearToken();
		}
	}
	/*
	char filename[256];
	char filename2[256];   // For bumpmap extraction two filename are extracted: need a second variable
	int tokenExtracted = 0;

	// Look for simple map
	tokenExtracted = sscanf(line.c_str()," map %s",filename);
	if (tokenExtracted == 1)
	{
		currentMaterial->mapFileName = string(filename);
		if (DEBUG)
			printf("Found map line#%d: '%s'\n",lineCounter,currentMaterial->mapFileName.c_str());
	}

	// Look for the specularmap
	size_t renderBumpIndex = line.find("renderbump");
	//printf("renderBumpIndex=%d, string::npos=%d\n",renderBumpIndex,string::npos);
	if (renderBumpIndex != string::npos)
		analyseRenderBump(line);


	tokenExtracted = 0;
	tokenExtracted = sscanf(line.c_str()," specularmap %s",filename);
	if (tokenExtracted == 1)
	{
		currentMaterial->specularMapFilename = string(filename);
		if (DEBUG)
			printf("Found specularMap line#%d: '%s'\n",lineCounter,currentMaterial->specularMapFilename.c_str());
	}




	// Look for diffusemap
	tokenExtracted = 0;
	tokenExtracted = sscanf(line.c_str()," diffusemap %s",filename);
	if (tokenExtracted == 1)
	{
		currentMaterial->diffuseMapFilename = string(filename);
		if (DEBUG)
			printf("Found diffuseMap line#%d: '%s'\n",lineCounter,currentMaterial->diffuseMapFilename.c_str());
	}



	// Look for bumpmap
	tokenExtracted = 0;
	tokenExtracted = sscanf(line.c_str()," bumpmap %*s (%s, heightmap (%s, ",filename,filename2);
	if (tokenExtracted == 2)
	{
		currentMaterial->normalMapFilename = string(filename);
		currentMaterial->heightMapFilename = string(filename2);
		if (DEBUG)
		{
			printf("Found bumpmap normal line#%d: '%s'\n",lineCounter,currentMaterial->normalMapFilename.c_str());
			printf("Found bumpmap height line#%d: '%s'\n",lineCounter,currentMaterial->heightMapFilename.c_str());
		}
	}
	*/
}

md5material* md5MaterialsLibrary::getMaterial(const char* name)
{
	return this->materials[string(name)];
}

void md5MaterialsLibrary::tokenize(const string& str,vector<string>& tokens,const string& delimiters )
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

void md5MaterialsLibrary::analyseRenderBump(string& line)
{
	vector<string> tokens;
	tokenize(line, tokens);

	int unsigned cursor=0;
	while(cursor < tokens.size() && tokens.at(cursor).compare("-aa"))
		cursor++;

	if (cursor >= tokens.size() ) //Not found !
		return;

	cursor++;

	int numberOrBumpInfo = atoi(tokens.at(cursor).c_str());

	cursor++;

	currentMaterial->bumpFilename = tokens.at(cursor);
}