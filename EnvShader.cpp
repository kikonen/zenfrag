#include "EnvShader.h"

EnvShader::EnvShader(char* vertexShader, char* fragmentShader) : ShadowShader(vertexShader,fragmentShader)
{
}

EnvShader::~EnvShader(void)
{
}

void EnvShader::enable(void)
{
	ShadowShader::enable();
}