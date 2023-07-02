#pragma once

struct Color
{
	float r,			
		  g,			
		  b,
		  a;

	Color(){}

	Color(float inR, float inG, float inB, float inA)
	{
		r = inR;
		g = inG;
		b = inB;
		a = inA;
	}
};