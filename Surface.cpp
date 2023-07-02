#include "Surface.h"
#include "glex_impl.h"
#include "CreatureShader.h"
#include "Vertex.h"

Surface::Surface(SurfaceType type)
{
	this->type = type;
}

Surface::~Surface(void)
{
}

void Surface::load(void)
{
	if (material != NULL)
		this->material->load();
	
	
	/*
			1---2
			|	|
			0---3
	*/
	// Setting vertex
	// Point 0
	
	vertices[0] = min.x;
	vertices[1] = min.y;
	vertices[2] = min.z;

	
	// Point 1
	vertices[3] = min.x;
	vertices[4] = max.y;
	if (type == WALL)
		vertices[5] = min.z;
	else
		vertices[5] = max.z;
	

	// Point 2
	vertices[6] = max.x;
	vertices[7] = max.y;
	vertices[8] = max.z;

	// Point 3
	vertices[9] = max.x;
	vertices[10] = min.y;
	if (type == WALL)
		vertices[11] = max.z;
	else
		vertices[11] = min.z;

	//Setting normal
	for (int i=0 ; i < 4 ; i++)
	{
		normals[i*3] = normal.x;
		normals[i*3+1] = normal.y;
		normals[i*3+2] = normal.z;
	}

	// Now tangents
	Vertex tangent;
	tangent.x = normal.x;
	tangent.y = normal.y;
	tangent.z = normal.z;

	if (type == WALL)
		tangent.rotateY(90);
	if (type == FLOOR)
		tangent.rotateZ(90);
	if (type == CEILING)
		tangent.rotateZ(-90);



	for (int i=0 ; i < 4 ; i++)
	{
		tangents[i*3] = tangent.x;
		tangents[i*3+1] = tangent.y;
		tangents[i*3+2] = tangent.z;
	}

	// Finally texture array

	//Point 0
	cooTexture[0] = 0.0f;
	cooTexture[1] = 0.0f;

	//Point 1
	cooTexture[2] = 0.0f;
	cooTexture[3] = densityy;

	//Point 2
	cooTexture[4] = densityx;
	cooTexture[5] = densityy;

	//Point 3
	cooTexture[6] = densityx;
	cooTexture[7] = 0.0f;

	// And requested to draw: elements array
	elements[0] = 0;
	elements[1] = 1;
	elements[2] = 2;
	elements[3] = 3;

	//Check orientation match with normal
	if (normal.z <= -0.9f || normal.x > 0.9f || this->type == FLOOR)
	{
		elements[0] = 0;
		elements[1] = 3;
		elements[2] = 2;
		elements[3] = 1;
	}


	glGenBuffersARB(1,&VBOvertices);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,VBOvertices);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB,4 * 3 * sizeof(float),vertices,GL_STATIC_DRAW_ARB);

	glGenBuffersARB(1,&VBOnormal);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,VBOnormal);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB,4 * 3 * sizeof(float),normals,GL_STATIC_DRAW_ARB);

	glGenBuffersARB(1,&VBOTexture);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,VBOTexture);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB,4 * 2 * sizeof(float),cooTexture,GL_STATIC_DRAW_ARB);

	glGenBuffersARB(1,&VBOTangent);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,VBOTangent);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB,4 * 3 * sizeof(float),tangents,GL_STATIC_DRAW_ARB);

	glGenBuffersARB(1,&VBOelements);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,VBOelements);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB,4 * 3 * sizeof(float),elements,GL_STATIC_DRAW_ARB);
}

void Surface::render()
{
	//return;
	//material->show();
	if (material != NULL)
	{
		glActiveTextureARB(GL_TEXTURE2);
		if (material->specularTextId != 0 )
			glBindTexture(GL_TEXTURE_2D,material->specularTextId);
		else
			glBindTexture(GL_TEXTURE_2D,0);
		

		glActiveTextureARB(GL_TEXTURE3);
		if (material->bumpTextId != 0 )
			glBindTexture(GL_TEXTURE_2D,material->bumpTextId);
		else
			glBindTexture(GL_TEXTURE_2D,0);

		glActiveTextureARB(GL_TEXTURE1);
		if (material->diffuseTextId != 0)
			glBindTexture(GL_TEXTURE_2D,material->diffuseTextId);
		else
			glBindTexture(GL_TEXTURE_2D,0);
	}
		
		

		glBindBufferARB(GL_ARRAY_BUFFER_ARB,VBOvertices);
		glVertexPointer (3, GL_FLOAT, 0, 0);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB,VBOnormal);
		glNormalPointer(GL_FLOAT,0, 0);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB,VBOTexture);
		glTexCoordPointer(2,GL_FLOAT,0,0);

		glVertexAttribPointerARB(Engine::engine->creatureShader->tangentLoc, 3, GL_FLOAT,GL_FALSE, 0, tangents);

		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,VBOelements);
		glDrawElements (GL_QUADS, 4 , GL_UNSIGNED_INT, 0);
		
		glActiveTextureARB(GL_TEXTURE0);

		Engine::engine->polygonRendered ++;
}

void Surface::update(){}
