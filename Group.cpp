#include "Group.h"
#include "Engine.h"
#include "glex_impl.h"



Group::Group(void)
{
	displayList=0;
	packed = false;
}

Group::Group(string name)
{
	displayList=0;
	this->name = name;
	packed = false;
}



Group::~Group(void)
{
}


void Group::pack()
{
	vertexArray = (float*)malloc( 3*3* faces.size() * sizeof(float));
	normalArray = (float*)malloc( 3*3* faces.size() * sizeof(float));
	texturArray = (float*)malloc( 2*3* faces.size() * sizeof(float));

	float* vertexArCursor = vertexArray;
	float* normalArCursor = normalArray;
	float* textCoAtCursor = texturArray;

	Face* currentFace = NULL;
	for(unsigned int i=0; i < faces.size() ; i++)
	{
		currentFace = faces.at(i);
		for (int j=0; j < 3 ; j++)
		{
			*vertexArCursor++ = currentFace->vertices[j].x;
			*vertexArCursor++ = currentFace->vertices[j].y;
			*vertexArCursor++ = currentFace->vertices[j].z;

			*normalArCursor++ = currentFace->normals[j].x;
			*normalArCursor++ = currentFace->normals[j].y;
			*normalArCursor++ = currentFace->normals[j].z;

			*textCoAtCursor++ = currentFace->textCoord[j].u;
			*textCoAtCursor++ = currentFace->textCoord[j].v;
		}
	}

	packed = true;

	
	glGenBuffersARB(1, &vboVertexId);
	//printf("vboVertexId =%d\n",vboVertexId);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboVertexId);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB,faces.size() * 3 * 3 * sizeof(float),vertexArray,GL_STATIC_DRAW_ARB);


	glGenBuffersARB(1, &vboNormalId);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboNormalId);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB,faces.size() * 3 * 3 * sizeof(float),normalArray,GL_STATIC_DRAW_ARB);

	glGenBuffersARB(1, &vboTexturId);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboTexturId);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB,faces.size() * 3 * 2 * sizeof(float),texturArray,GL_STATIC_DRAW_ARB);

	// Data were uploaded in the GPU we can discared them.
	free(vertexArray);
	free(normalArray);
	free(texturArray);
	

	//printf("vbo=%d",vboId);
}

void Group::renderViaArrayList()
{
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboVertexId);
	glVertexPointer(3,GL_FLOAT,0,0);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboNormalId);
	glNormalPointer(GL_FLOAT,0,0);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboTexturId);
	glTexCoordPointer(2,GL_FLOAT,0,0);
	

	glDrawArrays(GL_TRIANGLES,0,faces.size()*3);
}
