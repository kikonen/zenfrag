/*
 *  md5.cpp
 *  ShadowMap
 *
 *  Created by fabien sanglard on 16/12/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "md5.h"
#include "md5MaterialsLibrary.h"
#include "md5Animation.h"
#include "glex_impl.h"

// This is only used in the update method
#include "Timer.h"

#include "Engine.h"
#include "CreatureShader.h"

#define DEBUG 0

md5Object::md5Object()
{
	noDraw = -1;
	loadingStatus=1;
	animCursor=0;
	animation = NULL;
	//commandline = (char*)malloc(READ_BUFF_SIZE * sizeof(char));
}

md5Object::~md5Object(){}

void md5Object::set(const char* md5mesh, const  char* md5anim)
{
	this->md5mesh = md5mesh;
	this->md5anim = md5anim;
}

void md5Object::load()
{
	Engine::engine->creatureShader->tangentLoc ;

	string fileToOpen = Engine::engine->modelFolder ;
	fileToOpen += md5mesh;

	FILE* fp = fopen (fileToOpen.c_str(), "rb");
	if (!fp)
    {
		printf("Error: could not open file '%s'!\n", fileToOpen.c_str());
		return ;
    }
	
	if (DEBUG)
		printf("Succesfully opened file '%s'.\n", md5mesh);
	
	loadMesh(fp);
	fclose (fp);
	
	//Also need to load the animation
	if (md5anim.size() != 0)
	{
		animation = new md5Animation();
		animation->load(md5anim.c_str());
		if (!animation->loadingErrors)
			animation->generateAllSkeletons();

		//this->joints = animation->joints[100] ;
		
	}
}

void md5Object::loadMesh(FILE* fp)
{
	readVersion(fp);
	CHECK_LOADING_STATUS;
	
	readCommandLine(fp);
	CHECK_LOADING_STATUS;
	
	getJointsCount(fp);
	CHECK_LOADING_STATUS;
	
	getMeshesCount(fp);
	CHECK_LOADING_STATUS;
	
	readJoints(fp);
	CHECK_LOADING_STATUS;
	
	readMeshes(fp);
	CHECK_LOADING_STATUS;
	
	loadShaders();

	generateNormalsAndTangents();

	// VBO optimizations
	md5Mesh* currentMesh = NULL;
	for(int i = 0 ; i <meshesCount ; i++)
	{
		currentMesh = &meshes[i];
		//printf("mesh id=%d name=%s \n",i,currentMesh->shaderName);
		glGenBuffersARB(1, &vboVertexId[i]);
		//printf("	vboVertexId =%d size=%d\n",vboVertexId[i],currentMesh->verticesCount*3);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboVertexId[i]);
		//glBufferDataARB(GL_ARRAY_BUFFER_ARB,currentMesh->facesCount * 3 * 3 * sizeof(float),vertexArray[i],GL_DYNAMIC_DRAW_ARB);


		glGenBuffersARB(1, &vboNormalId[i]);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboNormalId[i]);
		//printf("	vboNormalId =%d aize=%d\n",vboNormalId[i],currentMesh->verticesCount*3);
		//glBufferDataARB(GL_ARRAY_BUFFER_ARB,currentMesh->facesCount * 3 * 3 * sizeof(float),normalArray[i],GL_DYNAMIC_DRAW_ARB);

		glGenBuffersARB(1, &vboTexturId[i] );
		//printf(" mesh %s has %d faces texture vbo id=%d\n",currentMesh->shaderName,currentMesh->facesCount,vboTexturId[i]);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboTexturId[i]);
		//printf("	vboTexturId =%d size=%d\n",vboTexturId[i],currentMesh->verticesCount*2);
		//printf(" binded !\n");
		glBufferDataARB(GL_ARRAY_BUFFER_ARB,currentMesh->verticesCount * 2 * sizeof(int),texturArraySkin[i],GL_STATIC_DRAW_ARB);

		glGenBuffersARB(1,&vboElementsId[i]);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboElementsId[i]);
		//printf("	vboElementId =%d, size=%d\n",vboElementsId[i],currentMesh->facesCount * 3);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB,currentMesh->facesCount * 3 *  sizeof(int),elementsSkin[i],GL_STATIC_DRAW_ARB);
	}
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);
	
}


//Private methods
// Read data from file
void md5Object::readVersion(FILE* fp)
{	
	
	
	while (sscanf (buff, " MD5Version %d",& this->version) != 1)
		fgets (buff, sizeof (buff), fp);

	if (version != MD5_VERSION)
	{
		/* Bad version */
		printf ( "Error: This version (%d) is not supported.\n");
		loadingStatus = MD5_LOADING_ERROR;
		return ;
	}
	else
	{
		if (DEBUG)
			printf ( "MD5 version (%d) is supported.\n",this->version);
	}
}

void md5Object::getJointsCount(FILE* fp)
{
	
	while(sscanf (buff, " numJoints %d", &this->jointsCount ) != 1)
		fgets (buff, sizeof (buff), fp);

	if (jointsCount == 0)
	{
		printf ( "Error: Could not find the number of joints in the model.\n");
		loadingStatus = MD5_LOADING_ERROR;
		return ;
	}
	
	if (DEBUG) 
		printf("Found %d joints declaration, allocating memory.\n",jointsCount);
	joints = (md5joint*)calloc( jointsCount , sizeof(md5joint) );
	
}

void md5Object::getMeshesCount(FILE* fp)
{
	while(sscanf (buff, " numMeshes %d", &this->meshesCount ) != 1)
		fgets (buff, sizeof (buff), fp);
	
	if (meshesCount == 0)
	{
		printf ( "Error: Could not find the number of meshes in the model.\n");
		loadingStatus = MD5_LOADING_ERROR;
		return ;
	}
	
	if (DEBUG)
		printf("Found %d meshes declaration, allocating memory.\n",meshesCount);
	
	// Now that we know how many meshes will be described we can allocated memory
	this->meshes = (md5Mesh*)calloc( meshesCount , sizeof(md5Mesh) );
	
	// Also allocating memory for the skeleton and skin array, used with VertexArrays;
	this->vertexArraySkin = (float**)calloc( meshesCount, sizeof(float*)  );
	this->elementsSkin = (int**)calloc( meshesCount, sizeof(int*)  );
	this->normalArraySkin = (float**)calloc( meshesCount, sizeof(float*)  );
	this->tangentArraySkin = (float**)calloc( meshesCount, sizeof(float*)  );
	this->texturArraySkin = (float**)calloc( meshesCount, sizeof(float*)  );

	vboVertexId = (GLuint*)calloc( meshesCount,sizeof(GLuint)  );
	vboNormalId = (GLuint*)calloc( meshesCount, sizeof(GLuint)  );
	vboTexturId = (GLuint*)calloc( meshesCount, sizeof(GLuint)  );
	vboTexturId = (GLuint*)calloc( meshesCount, sizeof(GLuint)  );
	vboElementsId = (GLuint*)calloc( meshesCount, sizeof(GLuint)  );
	
}

void md5Object::readCommandLine(FILE* fp)
{
	while (sscanf (buff, " commandline %s", this->commandline) != 1)
		fgets (buff, sizeof (buff), fp);
	
	if (DEBUG)
		printf("Command line is: %s.\n",commandline);
	// Nothing is done with the command line
}
void md5Object::readJoints(FILE* fp)
{
	// Seach for beginning of the joints
	while(strncmp (buff, "joints {", 8) != 0)
		fgets (buff, sizeof (buff), fp);
	
	// CurrentJoint is a pointer in the meshArray, we iterate through the join rows and update the pointer location
	md5joint* currentJoint = joints;
	for(int jointCounter=0 ; jointCounter < jointsCount; jointCounter++,currentJoint++)
	{
		fgets (buff, sizeof (buff), fp);
		sscanf (buff, "%s %d ( %f %f %f ) ( %f %f %f )",
				currentJoint->name, 
				&(currentJoint->parent), 
				&(currentJoint->position.x),
				&(currentJoint->position.y), 
				&(currentJoint->position.z),
				
				&currentJoint->orientation.x,
				&currentJoint->orientation.y, 
				&currentJoint->orientation.z);
		
		// Unpacking the last quaternion component
		currentJoint->orientation.computeW();
		
		if (DEBUG)
			printf("Read joint name:'%s', parent=%d, pos.x=%.2f ,pos.y=%.2f , pos.z=%.2f , q.x=%.2f ,q.y=%.2f ,q.z=%.2f\n",
			   currentJoint->name,
			   currentJoint->parent,
			   currentJoint->position.x,
			   currentJoint->position.y,
			   currentJoint->position.z,
			   currentJoint->orientation.x,
			   currentJoint->orientation.y,
			   currentJoint->orientation.z);
	}
}


void md5Object::readMeshes(FILE* fp)
{
	for( int meshCounter=0; meshCounter<meshesCount; meshCounter++)
	{
		if (DEBUG)
			printf("Reading mesh %d.\n",meshCounter);
		
		md5Mesh* currentMesh = &meshes[meshCounter];
		
		while(strncmp (buff, "mesh {", 6) != 0)
			fgets (buff, sizeof (buff), fp);

		// Read shader name
		while(sscanf (buff, " shader \"%s\" ", currentMesh->shaderName ) != 1)
			fgets (buff, sizeof (buff), fp);
		
		//A bit ugly but necessary: Remove the trailing '"' character
		currentMesh->shaderName[strlen(currentMesh->shaderName)-1] = '\0';

		if (DEBUG)
			printf("Shader name is '%s'.\n",currentMesh->shaderName);

		//Check if shader exist
		md5material* material= md5MaterialsLibrary::getInstance()->getMaterial(currentMesh->shaderName );
		if (material != NULL)
		{
			if (DEBUG)
			{
				printf("Shader was found in the library:\n");
				printf("	map:'%s'\n",material->mapFileName.c_str());
				printf("	diffuse:'%s'\n",material->diffuseMapFilename.c_str());
				printf("	specular:'%s'\n",material->specularMapFilename.c_str());
				printf("	normal:'%s'\n",material->normalMapFilename.c_str());
				printf("	height:'%s'\n",material->heightMapFilename.c_str());
			}
		}		

		//Read vertices
			while(sscanf (buff, " numverts %d", &currentMesh->verticesCount ) != 1)
				fgets (buff, sizeof (buff), fp);
		
			if (DEBUG)
				printf("%d vertices.\n",currentMesh->verticesCount);
		
			//Allocate space with this new information
			currentMesh->vertices = (md5Vertex*)calloc(currentMesh->verticesCount,sizeof(md5Vertex));


		
			md5Vertex* currentVertex = currentMesh->vertices;
			for(int vertexCounter=0;vertexCounter < currentMesh->verticesCount; )
			{
				int valueRead = sscanf (buff, " vert %*s ( %f %f ) %d %d", 
						//&currentVertex->textureCoor.u,
						&currentVertex->textureCoor.u, 
						&currentVertex->textureCoor.v, 
						&currentVertex->startWeightIndex, 
						&currentVertex->weightCount);
				
				
				
				
				fgets (buff, sizeof (buff), fp);
				
				//printf("Read value %d\n",valueRead);
				if (valueRead != 0) 
				{
					if (DEBUG)
						printf("Read vertex u=%.4f, v=%.4f, wStart=%d,wCount=%d.\n",currentVertex->textureCoor.u,currentVertex->textureCoor.v,currentVertex->startWeightIndex,currentVertex->weightCount);
					currentVertex++;
					vertexCounter++;
				}
				else
				{
					if (DEBUG)
						printf("Could not read value from mesh #%d, line#%d\n.",meshCounter,vertexCounter);
				}
			}
		//Read faces
			while(sscanf (buff, " numtris %d", &currentMesh->facesCount ) != 1)
				fgets (buff, sizeof (buff), fp);
		
			if (DEBUG)
				printf("%d faces.\n",currentMesh->facesCount);
			
			//Allocate space with this new information
			currentMesh->faces = (md5Face*)calloc(currentMesh->facesCount,sizeof(md5Face));
		
			md5Face* currentFace = currentMesh->faces;
			for(int faceCounter=0;faceCounter < currentMesh->facesCount; )
			{
				int valueRead = sscanf (buff, " tri %*s %d %d %d", 
						//&currentFace->index,
						&currentFace->point1Index, 
						&currentFace->point2Index, 
						&currentFace->point3Index);
				
				
							
				//printf("Read tri indexes %d, %d, %d\n",currentFace->point1Index,currentFace->point3Index,currentFace->point3Index);
				
				fgets (buff, sizeof (buff), fp);
				
				if (valueRead != 0) 
				{
					if (DEBUG)
						printf("Reading face: %d, indexes=%d,%d,%d\n",faceCounter,currentFace->point1Index,currentFace->point2Index,currentFace->point3Index);
					
					// De-reference
					//currentFace->point1 = currentMesh->vertices[currentFace->point1Index];
					//currentFace->point2 = currentMesh->vertices[currentFace->point2Index];
					//currentFace->point3 = currentMesh->vertices[currentFace->point3Index];
					
					faceCounter++,currentFace++;					
				}
				else
				{
					if (DEBUG)
						printf("Could not read value from mesh #%d, line#%d\n.",meshCounter,faceCounter);
				}
			}
		
		//Read weights
			while(sscanf (buff, " numweights %d", &currentMesh->weightsCount ) != 1)
				fgets (buff, sizeof (buff), fp);
		
			if (DEBUG)
				printf("%d weight.\n",currentMesh->weightsCount);

			//Allocate space with this new information
				currentMesh->weights = (md5Weight*)calloc(currentMesh->weightsCount,sizeof(md5Weight));
		
			md5Weight* currentWeight = currentMesh->weights;
			for(int weightCounter=0; weightCounter< currentMesh->weightsCount ;)
			{
				int readValue = sscanf (buff, " weight %*s %d %f ( %f %f %f )",
						//&currentWeight->index,
						&currentWeight->jointIndex, 
						&currentWeight->bias,
						&currentWeight->position.x, 
						&currentWeight->position.y, 
						&currentWeight->position.z);
				
				if (readValue != 0) 
				{
					if (DEBUG)
						printf("Reading weight: %d, jointIndex=%d,bias=%.4f\n",weightCounter,currentWeight->jointIndex,currentWeight->bias);
					
					// Dereferencing the joint  (Faitessss touuuurner !!!)
					//currentWeight->joint = &joints[currentWeight->jointIndex];
					
					weightCounter++;
					currentWeight++;
				}
				else
				{
					if (DEBUG)
						printf("Could not read value from mesh #%d, line#%d\n.",meshCounter,weightCounter);
				}
				
				fgets (buff, sizeof (buff), fp);
			}
	
}
	
	// Mesh file has been read, allocating space for openGL calls, vertexArray and elementArray
	// Looping to read all meshes.
	float** currentVertexArraySkin = vertexArraySkin;
	float** currentNormalArraySkin = normalArraySkin;
	float** currentTangentArraySkin = tangentArraySkin;
	int** currentElementSkin = elementsSkin;
	float** currenttextureArraySkin = texturArraySkin;
	
	
	for( int meshCounter=0; meshCounter< meshesCount; meshCounter++)
	{		
		md5Mesh* currentMesh = &meshes[meshCounter];
		
		*currentVertexArraySkin = (float*)calloc(currentMesh->verticesCount * 3,sizeof(float));
		currentVertexArraySkin++;
		
		*currentNormalArraySkin = (float*)calloc( currentMesh->verticesCount * 3, sizeof(float));
		currentNormalArraySkin++;

		*currentTangentArraySkin = (float*)calloc( currentMesh->verticesCount * 3, sizeof(float));
		currentTangentArraySkin++;
		
		*currentElementSkin = (int*)calloc(currentMesh->facesCount * 3,sizeof(int));
		currentElementSkin++;

		*currenttextureArraySkin = (float*)calloc(currentMesh->verticesCount * 2, sizeof(float) );
		currenttextureArraySkin++;
	}
		
	//Optimization, the element index to draw in openGL never change, we can set it up here:
	
	// Put elements inside elements array
	for (int meshIndex=0 ; meshIndex < meshesCount ; meshIndex++)
	{
		md5Mesh* currentMesh = &meshes[meshIndex];
		int* currentIndex = elementsSkin[meshIndex];
		for (int i=0; i < currentMesh->facesCount; i++ )
		{
			*currentIndex = currentMesh->faces[i].point1Index; currentIndex++;
			*currentIndex = currentMesh->faces[i].point2Index; currentIndex++;
			*currentIndex = currentMesh->faces[i].point3Index; currentIndex++;
			
			if (DEBUG)
			{
				printf("face Indexes p1=%d, p2=%d, p3=%d\n",currentMesh->faces[i].point1Index,currentMesh->faces[i].point2Index,currentMesh->faces[i].point3Index);
				printf("Reverse face Indexes p1=%d, p2=%d, p3=%d\n",*(currentIndex-3),*(currentIndex-2),*(currentIndex-1));
			}
		}
	}
	
	//Put texture coordinates in the array
	for (int meshIndex=0 ; meshIndex < meshesCount ; meshIndex++)
	{
		md5Mesh* currentMesh = &meshes[meshIndex];
		float* currentTextureArray = texturArraySkin[meshIndex];
		for(int i=0; i < currentMesh->verticesCount; i++)
		{
			md5Vertex* currentVertex = &currentMesh->vertices[i];
			*currentTextureArray++ = currentVertex->textureCoor.u;
			*currentTextureArray++ = currentVertex->textureCoor.v;
		}
	}
}

void md5Object::interolate(float tick)
{
}

void md5Object::prepareVertexArray(int meshIndex)
{
	
	md5Mesh* currentMesh = &meshes[meshIndex];
	
	if (DEBUG)
		printf("Preparing meshes #%d\n.",meshIndex);
	
	
	// Setup vertices 
	float* currentFloat = vertexArraySkin[meshIndex];
	float* currentNormal = normalArraySkin[meshIndex];
	float* currentTangent = tangentArraySkin[meshIndex];

	md5Vertex* currentVertex = currentMesh->vertices;
	for (int i = 0; i < currentMesh->verticesCount; i++, currentVertex++)
    {
		
		float3 finalVertex ;
		float3 finalNormal;
		float3 finalTangent;
		// Calculate final vertex to draw with weights 
		for (int weightCounter = 0; weightCounter < currentVertex->weightCount; weightCounter++)
		{
			md5Weight* weight = &currentMesh->weights[currentVertex->startWeightIndex + weightCounter];
			md5joint* joint= &joints[weight->jointIndex];
			
			// Calculate transformed vertex for this weight 
			if (DEBUG)
			{
				printf("weight->position x=%.2f,y=%.2f,z=%.2f\n",weight->position.x,weight->position.y,weight->position.z);
				printf("join->orientation x=%.2f,y=%.2f,z=%.2f,w=%.2f\n",joint->orientation.x,joint->orientation.y,joint->orientation.z,joint->orientation.w);
			}
			float3 displacement = joint->orientation.rotatePoint(weight->position);
			
			if (DEBUG)
				printf("displacement x=%.2f,y=%.2f,z=%.2f\n",displacement.x,displacement.y,displacement.z);
			//Quat_rotatePoint (joint->orient, weight->pos, wv);
			
			// The sum of all weight->bias should be 1.0 
			float3 t = joint->position + displacement;
			finalVertex += t * weight->bias;

			finalNormal += joint->orientation.rotatePoint(weight->normal) * weight->bias;
			finalTangent+= joint->orientation.rotatePoint(weight->tangent) * weight->bias;


		}

		finalNormal.normalize();
		finalTangent.normalize();
		
		if (DEBUG)
			printf("finalVertex x=%.2f,y=%.2f,z=%.2f\n",finalVertex.x,finalVertex.y,finalVertex.z);
		
		*currentFloat++ = finalVertex.x; //currentFloat++;
		*currentFloat++ = finalVertex.y; //currentFloat++;
		*currentFloat++ = finalVertex.z; //currentFloat++;	

		*currentNormal++ = finalNormal.x;
		*currentNormal++ = finalNormal.y;
		*currentNormal++ = finalNormal.z;

		*currentTangent++ = finalTangent.x;
		*currentTangent++ = finalTangent.y;
		*currentTangent++ = finalTangent.z;
		
		if (DEBUG)
			printf("Back finalVertex x=%.2f,y=%.2f,z=%.2f\n",*(currentFloat-3),*(currentFloat-2),*(currentFloat-1));

		
    }
	
	//Upload vertex array
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboVertexId[meshIndex]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB,currentMesh->verticesCount * 3  * sizeof(float),vertexArraySkin[meshIndex],GL_DYNAMIC_DRAW_ARB);

	//Upload normal array
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboNormalId[meshIndex]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB,currentMesh->verticesCount * 3  * sizeof(float),normalArraySkin[meshIndex],GL_DYNAMIC_DRAW_ARB);
	
	//printf("finished updating v\n");
}

void md5Object::renderNormals(int meshIndex)
{
	md5Mesh* currentMesh = &meshes[meshIndex];
	
	float* vertex = vertexArraySkin[meshIndex];
	float* normalV = normalArraySkin[meshIndex];
	glColor3f (0.0f, 1.0f, 0.0f);
	glBegin (GL_LINES);
	for (int i=0; i < currentMesh->verticesCount ; i++)
	{
		glVertex3f(vertex[i*3],vertex[i*3+1],vertex[i*3+2]);
		glVertex3f(vertex[i*3]+normalV[i*3],vertex[i*3+1]+normalV[i*3+1],vertex[i*3+2]+normalV[i*3+2]);


	}
	glEnd();
}

void md5Object::renderTangents(int meshIndex)
{
	md5Mesh* currentMesh = &meshes[meshIndex];
	glColor3f (1.0f, 0.0f, 0.0f);
	float* vertex = vertexArraySkin[meshIndex];
	float* tangentV = tangentArraySkin[meshIndex];
	
	glBegin (GL_LINES);
	for (int i=0; i < currentMesh->verticesCount ; i++)
	{
		glVertex3f(vertex[i*3],vertex[i*3+1],vertex[i*3+2]);
		glVertex3f(vertex[i*3]+tangentV[i*3],vertex[i*3+1]+tangentV[i*3+1],vertex[i*3+2]+tangentV[i*3+2]);


	}
	glEnd();
}

void md5Object::renderJoints()
{
	int i;
	
	/* Draw each joint */
	glPointSize (5.0f);
	glColor3f (1.0f, 0.0f, 0.0f);
	glBegin (GL_POINTS);
    for (i = 0; i < jointsCount; ++i)
		glVertex3f(joints[i].position.x,joints[i].position.y,joints[i].position.z);
	glEnd ();
	glPointSize (1.0f);
	
	/* Draw each bone */
	glColor3f (0.0f, 1.0f, 0.0f);
	glBegin (GL_LINES);
		for (i = 0; i < jointsCount; ++i)
		{
			if (joints[i].parent != -1)
			{
				md5joint* parentJoint = &joints[joints[i].parent];
				glVertex3f(parentJoint->position.x,parentJoint->position.y,parentJoint->position.z);
				glVertex3f(joints[i].position.x,joints[i].position.y,joints[i].position.z);
			}
		}
	glEnd();
}


int md5Object::lastSpecId=0;
int md5Object::lastDiffId=0;
int md5Object::lastBumpId=0;

void md5Object::renderSkin(int i)
{
	
	
		if (DEBUG)
		{
			printf("VertexArray:\n");
			for (int k=0 ; k < 50 ; k++)
				printf("%.2f,",vertexArraySkin[i][k]);	
			printf("\n");
		
			printf("Element array:\n");
			for (int k=0 ; k < 50 ; k++)
				printf("%d,",elementsSkin[i][k]);	
			printf("\n");
		
			//exit(0);
		}
		
		

		glActiveTextureARB(GL_TEXTURE2);
		if (meshes[i].shaderMaterial != NULL &&  meshes[i].shaderMaterial->specularTextId != 0 )
		{
			if (meshes[i].shaderMaterial->specularTextId != this->lastSpecId)
				glBindTexture(GL_TEXTURE_2D,meshes[i].shaderMaterial->specularTextId);
			lastSpecId = meshes[i].shaderMaterial->specularTextId;
		}
		else
			glBindTexture(GL_TEXTURE_2D,0);
		

		glActiveTextureARB(GL_TEXTURE3);
		if (meshes[i].shaderMaterial != NULL &&  meshes[i].shaderMaterial->bumpTextId != 0 )
		{
			if (meshes[i].shaderMaterial->bumpTextId != this->lastBumpId)
				glBindTexture(GL_TEXTURE_2D,meshes[i].shaderMaterial->bumpTextId);
			lastBumpId = meshes[i].shaderMaterial->bumpTextId;	
		}
		else
			glBindTexture(GL_TEXTURE_2D,0);

		glActiveTextureARB(GL_TEXTURE1);
		if (meshes[i].shaderMaterial != NULL && meshes[i].shaderMaterial->diffuseTextId != 0)
		{
			if (meshes[i].shaderMaterial->diffuseTextId != this->lastDiffId)
				glBindTexture(GL_TEXTURE_2D,meshes[i].shaderMaterial->diffuseTextId);
			lastDiffId= meshes[i].shaderMaterial->diffuseTextId;

		}
		else
			glBindTexture(GL_TEXTURE_2D,0);

		//for(int u=0;u<10;u+=2)
		//{
		//	printf("mesh #=%d, u=%f, v=%f\n",i,texturArraySkin[i][u],texturArraySkin[i][u+1]);
		//}
		

		glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboVertexId[i]);
		//glVertexPointer (3, GL_FLOAT, 0, vertexArraySkin[i]);
		glVertexPointer (3, GL_FLOAT, 0, 0);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboNormalId[i]);
		//glNormalPointer(GL_FLOAT,0, normalArraySkin[i]);
		glNormalPointer(GL_FLOAT,0, 0);

		//glTexCoordPointer(2,GL_FLOAT,0,texturArraySkin[i]);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboTexturId[i]);
		glTexCoordPointer(2,GL_FLOAT,0,0);

		//glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);

		glVertexAttribPointerARB(tangentLoc, 3, GL_FLOAT,GL_FALSE, 0, tangentArraySkin[i]);

		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,vboElementsId[i]);
		//glDrawElements (GL_TRIANGLES, meshes[i].facesCount * 3 , GL_UNSIGNED_INT, elementsSkin[i]);
		glDrawElements (GL_TRIANGLES, meshes[i].facesCount * 3 , GL_UNSIGNED_INT, 0);
		//glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);
		
		glActiveTextureARB(GL_TEXTURE0);
		
	
		Engine::engine->polygonRendered += (meshes[i].facesCount * 3);
}


void md5Object::render()
{
	//renderJoints();
	
	//glEnableVertexAttribArrayARB(tangentLoc);
	for(int i=0 ; i <   meshesCount ; i++)
	{
		//
		if (i == noDraw)
			continue;
			//printf("i=%d, meshSize=%d\n",i,meshes[i].facesCount);
		renderSkin(i);
		
		//Engine::engine->engine->creatureShader->disable();
		//renderNormals(i);
		//renderTangents(i);
		//Engine::engine->engine->creatureShader->enable();
	}
	//glDisableVertexAttribArrayARB(tangentLoc);
}


void md5Object::loadShaders()
{
	md5Mesh* currentMesh = NULL;

	for (int meshIndex=0 ; meshIndex < meshesCount ; meshIndex ++)
	{
		currentMesh = &meshes[meshIndex];
		currentMesh->shaderMaterial = md5MaterialsLibrary::getInstance()->getMaterial(currentMesh->shaderName);
		if (currentMesh->shaderMaterial == NULL)
			printf("Shader '%s'was NOT found in the library:\n",currentMesh->shaderName);\
		else
		{
			currentMesh->shaderMaterial->load();
			//currentMesh->shaderMaterial->show();
		}
	}
}

void md5Object::generateNormalsAndTangents()
{
	
	md5Mesh* currentMesh = NULL;
	for(int meshCounter=0; meshCounter < meshesCount ; meshCounter++)
	{
		currentMesh = &meshes[meshCounter];

		// Calculate the vertex positions
		//float3* vertexFinalPosition = (float3*)malloc(currentMesh->verticesCount * sizeof(float3)); 
		//memset(vertexFinalPosition,0,sizeof(float3));
		float3* vertexFinalPosition = (float3*)calloc(currentMesh->verticesCount,sizeof(float3)); 
		
		md5Vertex* currentVertex = NULL;
		for(int verticesCounter=0 ; verticesCounter < currentMesh->verticesCount ; verticesCounter++)
		{
			currentVertex = &currentMesh->vertices[verticesCounter];
			float3 finalVertex;
			for (int weightCounter = 0; weightCounter < currentVertex->weightCount; weightCounter++)
			{
				md5Weight* weight = &currentMesh->weights[currentVertex->startWeightIndex + weightCounter];
				md5joint* joint= &joints[weight->jointIndex];

				float3 displacement = joint->orientation.rotatePoint(weight->position);

				float3 t = joint->position + displacement;
				finalVertex += t * weight->bias;

				//Also zero normal for later use
				weight->normal = float3();
			}

			vertexFinalPosition[verticesCounter] = finalVertex;
		}

		// We now have all the vertex in final position

		// Calculate the nornals
		float3* normalsFinalPosition = (float3*)calloc(currentMesh->verticesCount, sizeof(float3)); 
		//memset(normalsFinalPosition,0,sizeof(float3));

		float3* tangentFinalPosition = (float3*)calloc(currentMesh->verticesCount, sizeof(float3)); 
		//memset(tangentFinalPosition,0,sizeof(float3));


		md5Face* currentFace = NULL;
		for(int facesCounter = 0; facesCounter < currentMesh->facesCount ; facesCounter++)
		{
			currentFace = &currentMesh->faces[facesCounter];

			// Normal part
			float3 v1 = vertexFinalPosition[currentFace->point3Index] - vertexFinalPosition[currentFace->point1Index];
			float3 v2 = vertexFinalPosition[currentFace->point2Index] - vertexFinalPosition[currentFace->point1Index];
			float3 normal = v1.crossProduct(v2);

			normalsFinalPosition[currentFace->point1Index] += normal;
			normalsFinalPosition[currentFace->point2Index] += normal;
			normalsFinalPosition[currentFace->point3Index] += normal;
			// End normal part


			// The following part is from "Mathematic for 3D programming" by Eric Lengyel
			// Tangent part

			float2 st1 = currentMesh->vertices[currentFace->point3Index].textureCoor - currentMesh->vertices[currentFace->point1Index].textureCoor;
			float2 st2 = currentMesh->vertices[currentFace->point2Index].textureCoor - currentMesh->vertices[currentFace->point1Index].textureCoor;

			// We now have a :

			//	[v1.x		v1.y		v1.z	]	=			[st1.s	st1.t][Tx		Ty		Tz]
			//	[v2.x		v2.y		v2.z	]				[st2.s	st2.t][Bx		By		Bz]

			//[Tx		Ty		Tz]	=			1/(st1.s * st2.t - st2.s * st1.t) *	[	 st2.t	-st1.t	][v1.x		v1.y		v1.z	]
			//[Bx		By		Bz]													[	-st2.s	 st1.s	][v2.x		v2.y		v2.z	]
				
			
			float coef = 1/ (st1.u * st2.v - st2.u * st1.v);
			float3 tangent;

			tangent.x = coef * ((v1.x * st2.v)  + (v2.x * -st1.v));
			tangent.y = coef * ((v1.y * st2.v)  + (v2.y * -st1.v));
			tangent.z = coef * ((v1.z * st2.v)  + (v2.z * -st1.v));


			tangentFinalPosition[currentFace->point1Index] += tangent;
			tangentFinalPosition[currentFace->point2Index] += tangent;
			tangentFinalPosition[currentFace->point3Index] += tangent;
			// End tangent part
		}

		// Normalize normals and tangent
		for(int verticesCounter=0 ; verticesCounter < currentMesh->verticesCount ; verticesCounter++)
		{
			normalsFinalPosition[verticesCounter].normalize();
			tangentFinalPosition[verticesCounter].normalize();
		}

		// Translating the normal orientation from object to joint space and Store normals inside weights, 
		for(int verticesCounter=0 ; verticesCounter < currentMesh->verticesCount ; verticesCounter++)
		{
			currentVertex = &currentMesh->vertices[verticesCounter];
			for (int weightCounter = 0; weightCounter < currentVertex->weightCount; weightCounter++)
			{
				md5Weight* weight = &currentMesh->weights[currentVertex->startWeightIndex + weightCounter];
				md5joint* joint= &joints[weight->jointIndex];

				float3 jointSpaceNormal = joint->orientation.copyAndInverse().rotatePoint(normalsFinalPosition[verticesCounter]);
				weight->normal += jointSpaceNormal;

				float3 jointSpaceTangent = joint->orientation.copyAndInverse().rotatePoint(tangentFinalPosition[verticesCounter]);
				weight->tangent += jointSpaceTangent; 
			}
		}

		// Normalize weight normals
		for (int weightCounter = 0; weightCounter < currentVertex->weightCount; weightCounter++)
		{
			md5Weight* weight = &currentMesh->weights[currentVertex->startWeightIndex + weightCounter];
			weight->normal.normalize();
			weight->tangent.normalize();
		}
		
		


		free(vertexFinalPosition);
		free(normalsFinalPosition);
		free(tangentFinalPosition);
	}
}



void md5Object::update()
{
	//printf("tick=%d\n",Timer::tick);
	if (animation != NULL)
	{
		animCursor = (animCursor + Timer::tick/1000.0 * animation->frameRate);
		//animCursor = 46.212;
		//printf(" %f out of %d\n",animCursor,animation->numFrames);
		if (animCursor >= animation->numFrames-1)
		{
			animCursor = 0;
			
			//joints = this->animation->joints[0];
			//this->position +=  this->animation->joints[animation->numFrames-1][0].position;
			//this->animation = NULL ;
			
		}
		else
		{
			int previousFrame = (int)animCursor;
			float percentageProgression = (animCursor - previousFrame);
			animation->update(previousFrame,percentageProgression,joints);
		}
	}

	for(int i=0 ; i <   meshesCount ; i++)
		prepareVertexArray(i);
	//else
	//	printf("anim null\n");
}