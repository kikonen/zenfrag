#include "WaveObject.h"
#include "Frustrum.h"
#include "Parser.h"
#include "ParserVertice.h"
#include "ParserTexture.h"
#include "ParserNormal.h"
#include "ParserFace.h"
#include "ParserMaterial.h"
#include "ParserMaterialSelector.h"
#include "ParserGroupSelector.h"

#include "Timer.h"
#include "SDL2/SDL.h"

#include "Engine.h"
#include "Camera.h"

#include "includes.h"


string WaveObject::pathContext = string(RESSOURCE_PATH);



WaveObject::WaveObject(float scaleFactor, Vertex offSet)
{
	parsers[string("v")] = new ParserVertice(this);
	parsers[string("vt")] = new ParserTexture(this);
	parsers[string("vn")] = new ParserNormal(this);
	parsers[string("f")] = new ParserFace(this);
	parsers[string("mtllib")] = new ParserMaterial(this);
	parsers[string("usemtl")] = new ParserMaterialSelector(this);
	parsers[string("g")] = new ParserGroupSelector(this);
	parsers[string("o")] = new ParserGroupSelector(this);

	this->scaleFactor=scaleFactor;
	this->offSet = offSet;
	displayListId=0;
	useDisplayList = 0;

	bsp = NULL;

}
WaveObject::~WaveObject(void){}

void tokenize(const string& str,vector<string>& tokens,const string& delimiters = " ")
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

void WaveObject::load(char* filename)
{
	string completeFilePath = pathContext + string(filename);

	printf("Loading OBJ file:'%s'.\n",completeFilePath.c_str());

   ifstream file;
   file.open(completeFilePath.c_str());


   if (!file)
	{
		printf("Could not load OBJ file:'%s'\n",completeFilePath.c_str());
		return;
	}

   this->name = (char*)malloc((strlen(filename)+1) * sizeof(char));
   strcpy(this->name,filename);
   //this->name = filename;

   string line;

   vector<string> tokens;
   Parser* parser;
   while (getline(file, line) )
   {
	 tokenize(line, tokens);
	 if (tokens.size() > 0)
	 {
		 parser = parsers[tokens.at(0)];
		 if (parser)

			parser->parse(tokens);
		 else
		 {
		//	printf("No parser found for %s\n",tokens.at(0).c_str());
		  }
	 }
	 tokens.clear();
   }

  // printf(" '%s' was loaded, optimizing step\n",completeFilePath.c_str() );

   // This minimize the number of openGL texture switchs.
   this->sortGroupPerTextureId();

  // printf(" '%s' was optimized\n",completeFilePath.c_str() );
}

void WaveObject::getAllBspGroups(BSP* bsp,vector<Group*>& toRender)
{
	for (unsigned int i=0; i < bsp->groups.size();i++)
			toRender.push_back(bsp->groups.at(i));

	if (bsp->front != NULL)
		getAllBspGroups(bsp->front,toRender);

	if (bsp->back != NULL)
		getAllBspGroups(bsp->back,toRender);
}



void WaveObject::buildSetOfGroupsToRender(BSP* bsp, vector<Group*>& toRender)
{

		CollisionDetection collideResult = Engine::engine->firstPerson->frustrum->checkAABBCollision(bsp->boudaryVolume);

		//printf("Collision detection = %d\n",collideResult);

		if (collideResult == COLL_OUT)
		{
			return;
		}

		if (collideResult == COLL_IN)
		{
			getAllBspGroups(bsp,toRender);
			return;
		}

		if (collideResult == COLL_INTERSECT)
		{

			if (bsp->front == NULL && bsp->back == NULL)
			{
				getAllBspGroups(bsp,toRender);
				return;
			}

			if (bsp->front != NULL)
				buildSetOfGroupsToRender(bsp->front,toRender);

			if (bsp->back != NULL)
				buildSetOfGroupsToRender(bsp->back,toRender);
		}






}


bool myfunction (Group* i,Group* j){
	return (i->material->textureId <j->material->textureId);
}

void WaveObject::render()
{

	//printf("Rendering object = '%s'\n",this->name);
	if (bsp != NULL)
	{
		vector<Group*> groupsToRender;

	//	if (TRACE)
//			printf("Building group to render.\n");
		buildSetOfGroupsToRender(bsp,groupsToRender);
//		if (TRACE)
//			printf("Done Building group to render.\n");

		//printf("Number of groups to render for Tenso %d\n",groupsToRender.size());

		sort(groupsToRender.begin(),groupsToRender.end(),myfunction);

		renderDirectMode(groupsToRender);

		return;
	}

	if (!useDisplayList)
	{
		renderDirectMode(groups);
		return;
	}
	else
	{
		if(displayListId != 0)
		{
			glCallList(displayListId);
			return;
		}
		else
		{
			displayListId = glGenLists(1);

			glNewList(displayListId,GL_COMPILE);
			renderDirectMode(groups);
			glEndList();
		}
	}
}

void WaveObject::renderDirectMode(vector<Group*>& groupIn)
{


	Group* group = NULL;
	Material* currentMaterial = NULL;


	for(unsigned int i=0 ; i < groupIn.size(); i++)
	{
		group = groupIn.at(i);

		//printf("Drawing group %s\n",group->name.c_str());
		if (currentMaterial != group->material)
		{
			currentMaterial = group->material;
			glBindTexture(GL_TEXTURE_2D,currentMaterial->textureId);
			Engine::engine->textureSwitchs++;
		//	printf("Using material %s, textureName=%s,textureId=%d\n",currentMaterial->name.c_str(),currentMaterial->textureFileName.c_str(),currentMaterial->textureId);
		}

			/*
				glBegin(GL_TRIANGLES);
				Face* face = NULL;
				for(unsigned int j=0; j < group->faces.size();j++)// && (SDL_GetTicks() /500) > j; j++)
				{
					face = group->faces.at(j);

						for (unsigned int w=0 ; w < 3 ; w ++)
						{
					//		printf("Using material textCood u=%f, v=%f\n",face->textCoord[w].u,face->textCoord[w].v);
							glTexCoord2f(face->textCoord[w].u,face->textCoord[w].v);
							glNormal3f(face->normals[w].x,face->normals[w].y,face->normals[w].z);
							glVertex3f(face->vertices[w].x,face->vertices[w].y,face->vertices[w].z);

						}
						Engine::engine->polygonRendered++;

				}
				glEnd();
			*/


		if (!group->packed)
			group->pack();
		group->renderViaArrayList();
		Engine::engine->polygonRendered+=group->faces.size();


	}

}



void WaveObject::sortGroupPerTextureId()
{
	sort(groups.begin(),groups.end(),myfunction);
}



void WaveObject::buildBsp()
{
	Vertex min;
	Vertex max;

	printf("	'%s' will use Binary Space Partition.\n",this->name);

	Group* currentGroup = NULL;
	Face* currentFace = NULL;
	for (unsigned int groupI=0;groupI < groups.size() ; groupI++)
	{
		currentGroup = groups.at(groupI);
		for(unsigned int faceI=0 ; faceI < currentGroup->faces.size(); faceI++)
		{
			currentFace = currentGroup->faces.at(faceI);
			for (unsigned vertexI=0 ; vertexI < 3 ; vertexI++)
			{
				if (min.x > currentFace->vertices[vertexI].x)
					min.x = currentFace->vertices[vertexI].x;

				if (min.y > currentFace->vertices[vertexI].y)
					min.y = currentFace->vertices[vertexI].y;

				if (min.z > currentFace->vertices[vertexI].z)
					min.z = currentFace->vertices[vertexI].z;

				if (max.x < currentFace->vertices[vertexI].x)
					max.x = currentFace->vertices[vertexI].x;

				if (max.y < currentFace->vertices[vertexI].y)
					max.y = currentFace->vertices[vertexI].y;

				if (max.z < currentFace->vertices[vertexI].z)
					max.z = currentFace->vertices[vertexI].z;
			}
		}
	}

	this->bsp = new BSP();
	this->bsp->boudaryVolume = AABB(min,max);
	this->bsp->groups = this->groups;

	refineBSP(bsp,0);

	//printf(" BSP done for object %s, min x=%f,y=%f,z=%f   max x=%f,y=%f,z=%f\n",this->name,min.x,min.y,min.z,max.x,max.y,max.z);
}

void WaveObject::refineBSP(BSP* bsp, int depth)
{
	if (depth > 3)
		return;
	// Define cutting plan

	//printf("bsp box\n");
	//bsp->boudaryVolume.dump();


	// Cut AABB in two AABBs over this plan
	AABB farBox = AABB(
						(bsp->boudaryVolume.boundaries[FBR]+bsp->boudaryVolume.boundaries[NBR])
						/2.0,
						bsp->boudaryVolume.boundaries[FTL]);

	//printf("far box\n");
	//farBox.dump();

	AABB nearBox = AABB(bsp->boudaryVolume.boundaries[NBR],
						(bsp->boudaryVolume.boundaries[FTL]+bsp->boudaryVolume.boundaries[NTL])
						 /2
						 );

	//printf("near box\n");
	//nearBox.dump();

	// Create two new BSP and attribute newly created AABB
	bsp->front = new BSP();
	bsp->front->boudaryVolume = farBox;

	bsp->back = new BSP();
	bsp->back->boudaryVolume = nearBox;

	Plan plan = Plan(Vertex(0,0,1),farBox.boundaries[NTR]);
	//printf("Splitting plane: point x=%f,y=%f,z=%f\n",farBox.boundaries[NTR].x,farBox.boundaries[NTR].y,farBox.boundaries[NTR].z);
	//printf("Splitting plane: normal x=%f,y=%f,z=%f d=%f\n",plan.normalVector.x,plan.normalVector.y,plan.normalVector.z,plan.d);

	// Go over each groups and dispatch them between new AABBs
	dispatch(bsp->groups,plan,bsp->front,bsp->back);

	// Check in one of the two BSP is empty, if so, delete it.
	if (bsp->front->groups.size() == 0)
		bsp->front = NULL;
	else
		refineBSP(bsp->front,depth+1);

	if (bsp->back->groups.size() == 0)
		bsp->back = NULL;
	else
		refineBSP(bsp->back,depth+1);

}


void WaveObject::dispatch(vector<Group*>& groups,Plan& plan, BSP* front, BSP* back)
{
	for (unsigned int i=0; i < groups.size(); i ++)
		splitGroup(groups.at(i),plan,front->groups,back->groups);

	groups.clear();
}

void WaveObject::splitGroup(Group* group,Plan& plan, vector<Group*>& front,vector<Group*>& back)
{
	Group* frontGroup = new Group();
	frontGroup->material = group->material;

	Group* backGroup = new Group();
	backGroup->material = group->material;

	Face* currentFace = NULL;
	for(unsigned int i=0 ; i < group->faces.size() ; i++)
	{
		currentFace = group->faces.at(i);
		splitFace(currentFace,plan,frontGroup,backGroup);
	}

	if (frontGroup->faces.size() != 0)
		front.push_back(frontGroup);
	else
		delete frontGroup;

	if (backGroup->faces.size() != 0)
		back.push_back(backGroup);
	else
		delete backGroup;

	//printf("Group splitted front=%d, back=%d\n",front.size(),back.size());
}

void WaveObject::splitFace(Face* face,Plan& plan,Group* front, Group* back)
{

	float distanceVo = plan.distanceFromPoint(face->vertices[0]);
	float distanceV1 = plan.distanceFromPoint(face->vertices[1]);
	float distanceV2 = plan.distanceFromPoint(face->vertices[2]);

	// The polygon is NO CUT by plan  (or 2 edges at max are on the cutting plan)
	//IN
	if (distanceVo >= 0 &&
		distanceV1 >= 0 &&
		distanceV2 >= 0 )
	{
		front->faces.push_back(face);
		return;
	}

	//OUT
	else if (distanceVo <= 0 &&
			 distanceV1 <= 0 &&
			 distanceV2 <= 0 )
	{
		back->faces.push_back(face);
		return;

	}

	cutFaceViaPlan(face,front,back,plan);

}

void WaveObject::cutFaceViaPlan(Face* faceToCut,Group* front, Group* back, Plan& plan )
{
	vector<Vertex> frontVertex ;
	vector<Vertex> frontNormal ;
	vector<TextCoord> frontTextCoo ;

	vector<Vertex> backVertex ;
	vector<Vertex> backNormal ;
	vector<TextCoord> backTextCoo ;


	for(int i=0; i < 3 ; i ++)
	{
		if (plan.distanceFromPoint(faceToCut->vertices[i]) >0 )
		{
			frontVertex.push_back(faceToCut->vertices[i]);
			frontNormal.push_back(faceToCut->normals[i]);
			frontTextCoo.push_back(faceToCut->textCoord[i]);
		}
		else
		{
			backVertex.push_back(faceToCut->vertices[i]);
			backNormal.push_back(faceToCut->normals[i]);
			backTextCoo.push_back(faceToCut->textCoord[i]);
		}
	}

	if (frontVertex.size() == 3 || backVertex.size() == 3)
	{
		printf("Error in face partitionning##################### front=%d, back=%d\n",frontVertex.size(),backVertex.size());
		for (unsigned int i=0; i< frontVertex.size() ; i++)
			printf("Error in face partitionning frontVertex d=%f\n",plan.distanceFromPoint(frontVertex.at(i)));
		for (unsigned int i=0; i< backVertex.size() ; i++)
			printf("Error in face partitionning backVertex d=%f\n",plan.distanceFromPoint(backVertex.at(i)));
		return;
	}

	unsigned int frontSize = frontVertex.size();
	unsigned int backSize = backVertex.size();
	for(unsigned int i=0 ; i < frontSize; i ++)
	{
		for(unsigned int j=0 ; j < backSize; j ++)
		{
			float u = processIntesection(frontVertex.at(i),backVertex.at(i),plan);
			if (u >= 0 && u <= 1)
			{
				Vertex intersection = (backVertex.at(j) - frontVertex.at(i)  )  *    u + frontVertex.at(i)  ;
				Vertex intersectionN = frontNormal[i]  * (1-u)  +   backNormal[j] * u ;
				//printf("Slicing text coo: frontTextCoo[i]=(%f,%f) and backTextCoo[j]=(%f,%f)\n",frontTextCoo[i].u,frontTextCoo[i].v,backTextCoo[j].u,backTextCoo[j].v);

				TextCoord intersectionT = frontTextCoo[i] * (1-u)  +   backTextCoo[j]* u;
				//printf("Slicing text coo result (t=%f) = %f, %f:\n",u,intersectionT.u,intersectionT.v);

				frontVertex.push_back(intersection);
				backVertex.push_back(intersection);

				frontNormal.push_back(intersectionN);
				backNormal.push_back(intersectionN);

				frontTextCoo.push_back(intersectionT);
				backTextCoo.push_back(intersectionT);
			}
		}
	}


	if (frontVertex.size() == 3 || frontVertex.size() == 4)
	{
		Face* newFace = new Face();
		for (unsigned int i=0 ; i < 3 ; i++)
		{
			newFace->vertices[i] = frontVertex[i];
			newFace->normals[i] = frontNormal[i];
			newFace->textCoord[i] = frontTextCoo[i];
		}
		front->faces.push_back(newFace);
	}


	if (frontVertex.size() == 4 )
	{
		Face* newFace = new Face();
		for (unsigned int i=1 ; i < 4 ; i++)
		{
			newFace->vertices[i-1] = frontVertex[i];
			newFace->normals[i-1] = frontNormal[i];
			newFace->textCoord[i-1] = frontTextCoo[i];
		}
		front->faces.push_back(newFace);
	}



	if (backVertex.size() == 3 || backVertex.size() == 4)
	{
		Face* newFace = new Face();
		for (unsigned int i=0 ; i < 3 ; i++)
		{

			newFace->vertices[i] = backVertex[i];
			newFace->normals[i] = backNormal[i];
			newFace->textCoord[i] = backTextCoo[i];
		}
		back->faces.push_back(newFace);
	}




	if (backVertex.size() == 4)
	{
		Face* newFace = new Face();
		for (unsigned int i=1 ; i < 4 ; i++)
		{
			newFace->vertices[i-1] = backVertex[i];
			newFace->normals[i-1] = backNormal[i];
			newFace->textCoord[i-1] = backTextCoo[i];
		}
		back->faces.push_back(newFace);
	}

}


float WaveObject::processIntesection(Vertex& point1, Vertex& point2, Plan& plan)
{
	   //  t={d-a x_a - b y_a - c z_a \over a (x_b-x_a)+ b (y_b-y_a) + c(z_b-z_a)}.


	float nomimator  =	- plan.d
						- plan.normalVector.x * point1.x
						- plan.normalVector.y * point1.y
						- plan.normalVector.z * point1.z
						;


	float demonimator = plan.normalVector.x * ( point2.x - point1.x) +
						plan.normalVector.y * ( point2.y - point1.y) +
						plan.normalVector.z * ( point2.z - point1.z) ;

	if ( demonimator == 0)
		return 0;


	//	printf("res = %f\n",nomimator/demonimator);

	return nomimator/demonimator;
}

