#pragma once

#include "3DDefinitions.h"
#include "includes.h"

class Group
{
public:

	string name;
	vector<Face*> faces;
	Material* material;
	int displayList;

	// For vertexArray mode
	float* vertexArray;
	float* normalArray;
	float* texturArray;
	
	// For vbo mode
	GLuint vboVertexId;
	GLuint vboNormalId;
	GLuint vboTexturId;

	bool packed;

	Group(void);
	Group(string name);
	~Group(void);

	void pack();
	void renderViaArrayList();
};
