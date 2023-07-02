#include "ParserMaterial.h"
#include "includes.h"


#include "ParserMTLNewMaterial.h"
#include "ParserMTLKd.h"
#include "ParserMTLKa.h"
#include "ParserMTLKs.h"
#include "ParserMTLD.h"
#include "ParserMTLmap_Kd.h"
#include "WaveObject.h"

using namespace std;

ParserMaterial::ParserMaterial(WaveObject* object)
{
	this->object = object;

	parsers[string("newmtl")] = new ParserMTLNewMaterial(object);
	parsers[string("Kd")] = new ParserMTLKd(object);
	parsers[string("Ka")] = new ParserMTLKa(object);
	parsers[string("Ks")] = new ParserMTLKs(object);
	parsers[string("d")] = new ParserMTLD(object);
	parsers[string("map_Kd")] = new ParserMTLmap_Kd(object);
}

void ParserMaterial::parse(vector<string>& line)
{
	ifstream file;

	string fileToOpen = WaveObject::pathContext + line.at(1);

	file.open(fileToOpen.c_str());

	//printf("Loading MTL file:'%s'\n",fileToOpen.c_str());
	if (!file)
	{
		printf("Could not locate MTL file:'%s'\n",fileToOpen.c_str());
		return;
	}

	string lineRead;
	vector<string> tokens;
   Parser* parser;	
   while (getline(file, lineRead) )
   {
	 tokenize(lineRead, tokens, " ");
	 if (tokens.size() > 0)
	 {
		 /*
		 if (*(tokens.at(0).c_str()) == '#')
		 {
			printf("Comment found\n");
			continue;
		 }
		 */

		 parser = parsers[tokens.at(0)];
		// printf("checking parser for %s\n",parser->parse(tokens));
		 if (parser)
		 {
		//	 printf("Parser found\n");
			parser->parse(tokens);
		}
		 else
		 {
			printf("No parser found for %s\n",tokens.at(0).c_str());
		}
		//cout <<  tokens.at(0) << "\n" ;
	 }
	 tokens.clear();
   }
}

