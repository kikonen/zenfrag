#pragma once
#include "shaderprogram.h"
#include "float2.h"

class ShadowShader : public ShaderProgram
{
public:
	ShadowShader(char* vertexShader, char* fragmentShader);
	virtual void enable();
	~ShadowShader(void);

	GLint textureUnitUniform;
	float2 textureUnit;
};
