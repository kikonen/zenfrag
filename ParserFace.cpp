#include "ParserFace.h"
#include "WaveObject.h"

ParserFace::ParserFace(WaveObject* object)
{
	this->object = object;
}



void ParserFace::parse(vector<string>& line)
{
	static int indice_vertice;
	static int indice_texture;
	static int indice_normal;
	
	Face* face = new Face();

	for (int i = 1; i < 4 ;  i++)
	{
		static vector<string> tokens;
		tokenize(line.at(i),tokens,"/");
		sscanf(tokens.at(0).c_str(), "%d", &indice_vertice);

		if (tokens.size() == 3)
		{
			sscanf(tokens.at(1).c_str(), "%d", &indice_texture);
			sscanf(tokens.at(2).c_str(), "%d", &indice_normal);
			face->vertices[i-1] = *(object->vertices.at(indice_vertice-1));
			face->textCoord[i-1] = *(object->textCoords.at(indice_texture-1));
			face->normals[i-1] = *(object->normals.at(indice_normal-1));	
		}

		if (tokens.size() == 2)
		{
			sscanf(tokens.at(1).c_str(), "%d", &indice_normal);
			face->vertices[i-1] = *(object->vertices.at(indice_vertice-1));
			face->textCoord[i-1] = TextCoord();
			face->normals[i-1] = *(object->normals.at(indice_normal-1));
		}
		

		


		tokens.clear();
	}

	object->faces.push_back(face);
	object->currentGroup->faces.push_back(face);
}


