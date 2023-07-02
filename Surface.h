#pragma once

#include "float3.h"
#include "md5definitions.h"
#include "includes.h"


class Surface
{
public:

	enum SurfaceType { WALL, CEILING, FLOOR};


	Surface(SurfaceType type);
	~Surface(void);
	void load(void);
	void render();
	void update();

	float3 min;
	float3 max;
	float3 normal;
	md5material* material;
	int densityx;
	int densityy;

	SurfaceType type;

private:
	
	GLuint VBOvertices;
	GLuint VBOnormal;
	GLuint VBOTexture;
	GLuint VBOTangent;
	GLuint VBOelements;

	float vertices[12];
	float cooTexture[8];
	float normals[12];
	float tangents[12];
	GLuint elements[4];
};
