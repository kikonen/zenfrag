#pragma once
#include "ShadowShader.h"

class EnvShader : public ShadowShader
{
public:
	EnvShader(char* vertexShader, char* fragmentShader);
	~EnvShader(void);
	virtual void enable();
	//virtual void bindUniformsAndAttribute(void);
};
