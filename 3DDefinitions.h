#pragma once

#define _USE_MATH_DEFINES
#include "includes.h"
#include "Color.h"
#include "Vertex.h"

//enum ViewingPlan  { BOTTOM=0, TOP=1, LEFT=2, RIGHT=3, NEAR=4, FAR=5};
#define ViewingPlan int
#define BOTTOM 0
#define TOP 1
#define LEFT 2
#define RIGHT 3
#define NEAR 4
#define FAR 5


enum CollisionDetection { COLL_OUT=0, COLL_INTERSECT=1, COLL_IN=2};

using namespace std;









struct TextCoord
{
	float u;
	float v;

	TextCoord()
	{}

	TextCoord(float inU, float inV)
	{
		u = inU;
		v = inV;
	}

	TextCoord operator* (const float k) const 
   {
	   return TextCoord (u *k, v * k);
   }

	TextCoord operator/ (const float k) const 
   {
	   return TextCoord (u /k, v / k);
   }

	TextCoord operator+ (const TextCoord& k) const 
   {
	   return TextCoord (u +k.u, v + k.v);
   }
};

struct Face
{
	Vertex vertices[3];
	Vertex normals[3];
	TextCoord textCoord[3];

	Face()
	{
	}
} ;

struct Material
{
	string mapKd;
	int textureId;
	string textureFileName;
	Color* kd;
	Color* ka;
	Color* ks;
	string name;

	Material()
	{
	}

};




struct Plan
{
	Plan()
	{

	}

	Plan(Vertex& vector1,Vertex& vector2,Vertex& point)
	{
		normalVector = vector1.crossProduct(vector2);
		d = - normalVector.dotProduct(point);
	}

	Plan(Vertex normalVector,Vertex& point)
	{
		this->normalVector = normalVector;
		d = - normalVector.dotProduct(point);
	}

	Vertex normalVector;
	float d;

	float distanceFromPoint(Vertex& point)
	{
		return normalVector.dotProduct(point) + d;
	}
};

enum BoxBoundaries { FTR=0, FTL=1, FBR=2, FBL=3, NTR=4, NTL=5, NBR=6, NBL=7};

struct AABB
{
	Vertex boundaries[8];

	AABB(){}

	AABB(Vertex& iFTR, Vertex& iFTL ,Vertex& iFBR,Vertex& iFBL,Vertex&  iNTR,Vertex&  iNTL,Vertex&  iNBR,Vertex&  iNBL)
	{
		boundaries[FTR] = iFTR;
		boundaries[FTL] = iFTL;
		boundaries[FBR] = iFBR;
		boundaries[FBL] = iFBL;
		boundaries[NTR] = iNTR;
		boundaries[NTL] = iNTL;
		boundaries[NBR] = iNBR;
		boundaries[NBL] = iNBL;
	}

	AABB(Vertex min, Vertex max )
	{
		boundaries[FTR] = Vertex(min.x,max.y,max.z);
		boundaries[FTL] = Vertex(max);
		boundaries[FBR] = Vertex(min.x,min.y,max.z);
		boundaries[FBL] = Vertex(max.x,min.y,max.z);

		boundaries[NTR] = Vertex(min.x,max.y,min.z);
		boundaries[NTL] = Vertex(max.x,max.y,min.z);
		boundaries[NBR] = Vertex(min);
		boundaries[NBL] = Vertex(max.x,min.y,min.z);
	}

	AABB(Vertex* inTab)
	{
		boundaries[FTR] = inTab[FTR];
		boundaries[FTL] = inTab[FTL];
		boundaries[FBR] = inTab[FBR];
		boundaries[FBL] = inTab[FBL];
		boundaries[NTR] = inTab[NTR];
		boundaries[NTL] = inTab[NTL];
		boundaries[NBR] = inTab[NBR];
		boundaries[NBL] = inTab[NBL];
	}

	void dump()
	{
		printf("boundaries[FTR] x=%f, y=%f, z=%f\n",boundaries[FTR].x,boundaries[FTR].y,boundaries[FTR].z);
		printf("boundaries[FTL] x=%f, y=%f, z=%f\n",boundaries[FTL].x,boundaries[FTL].y,boundaries[FTL].z);
		printf("boundaries[FBR] x=%f, y=%f, z=%f\n",boundaries[FBR].x,boundaries[FBR].y,boundaries[FBR].z);
		printf("boundaries[FBL] x=%f, y=%f, z=%f\n",boundaries[FBL].x,boundaries[FBL].y,boundaries[FBL].z);
		printf("boundaries[NTR] x=%f, y=%f, z=%f\n",boundaries[NTR].x,boundaries[NTR].y,boundaries[NTR].z);
		printf("boundaries[NTL] x=%f, y=%f, z=%f\n",boundaries[NTL].x,boundaries[NTL].y,boundaries[NTL].z);
		printf("boundaries[NBR] x=%f, y=%f, z=%f\n",boundaries[NBR].x,boundaries[NBR].y,boundaries[NBR].z);
		printf("boundaries[NBL] x=%f, y=%f, z=%f\n",boundaries[NBL].x,boundaries[NBL].y,boundaries[NBL].z);
	}
};

#define TRACE 1

