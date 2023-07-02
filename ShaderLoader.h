#pragma once

#include "includes.h"
#include "glex_impl.h"



class ShaderLoader
{
public:
	ShaderLoader(void);
	~ShaderLoader(void);

	
	GLhandleARB loadShader(char* filename, unsigned int type);
};
