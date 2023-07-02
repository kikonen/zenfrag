#pragma once

#include "includes.h"

#define ANG2RAD 3.14159265358979323846/180.0
#define DEGRES_TO_RAD(x) (x * 3.14159265358979323846/180.0) 

struct Vertex
{
	float x,			
		  y,			
		  z;	

	Vertex()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	Vertex(float inX, float inY, float inZ)
	{
		x = inX;
		y = inY;
		z = inZ;
	}




	void normalize()
	{
		float norm = sqrt(x*x+y*y+z*z );
		this->x /= norm;
		this->y /= norm;
		this->z /= norm;
	}

	Vertex operator+ (const Vertex &k) const 
   {
	   return Vertex (x + k.x, y + k.y, z + k.z);
   }

	void operator= (const Vertex &k)  
   {
	   x = k.x;
	   y = k.y;
	   z = k.z;
   }

	Vertex operator- (const Vertex &k) const 
   {
	   return Vertex (x - k.x, y - k.y, z - k.z);
   }

	Vertex operator/ (const Vertex &k) const 
   {
	   return Vertex (x / k.x, y / k.y, z / k.z);
   }

	Vertex operator* (const int k) const 
   {
	   return Vertex (x *k, y * k, z * k);
   }

	void operator*= (const float k)  
   {
	   x *=k;
	   y *= k;
	   z *= k;
   }

	Vertex operator* (const float k) const 
   {
	   return Vertex (x *k, y * k, z * k);
   }

	Vertex operator/ (const float k) const 
   {
	   return Vertex (x /k, y / k, z / k);
   }

	Vertex crossProduct (const Vertex &b) const 
   {
	   return Vertex (y*b.z - z*b.y,  
		              z*b.x - x*b.z, 
					  x*b.y - y*b.x);
   }


	void rotateX (double degres) 
   {
	   float tmpY = y;
	   degres = DEGRES_TO_RAD(degres);
	   y = (float)( cos(degres)*y    + sin(degres)*z);
	   z = (float)(-sin(degres)*tmpY + cos(degres)*z);
   }


	void rotateY (double degres) 
   {
	   float tmpX = x;
	   degres = DEGRES_TO_RAD(degres);
	   x = (float)(cos(degres)*x    - sin(degres)*z);
	   z = (float)(sin(degres)*tmpX + cos(degres)*z);
   }

	void rotateZ (double degres) 
   {
	   float tmpX = x;
	   degres = DEGRES_TO_RAD(degres);
	   x = (float)( cos(degres)*x    + sin(degres)*y);
	   y = (float)(-sin(degres)*tmpX + cos(degres)*y);
   }

	float dotProduct(const Vertex &point) const 
	{
		return (x*point.x + y*point.y + z*point.z);
	}

	void dump()
	{
		printf("x=%f,y=%f.z=%f\n",x,y,z);
	}
} ;