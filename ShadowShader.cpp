#include "ShadowShader.h"
#include "main.h"
#include "Engine.h"

ShadowShader::ShadowShader(char* vertexShader, char* fragmentShader) : ShaderProgram(vertexShader,fragmentShader)
{
	textureUnit.u = 1.0f/(Engine::engine->renderWidth * Engine::engine->fboShadowBuffersRatio);
	textureUnit.v = 1.0f/(Engine::engine->renderHeight* Engine::engine->fboShadowBuffersRatio);

//	printf("textureUnit.u = %.8f, textureUnit.v = %.8f\n",textureUnit.u,textureUnit.v  );
//	printf("manual Unit.u = %.8f, textureUnit.v = %.8f\n",1.0f/4096.0f,1.0f/3072.0f );

	textureUnitUniform = glGetUniformLocationARB(shaderId,"textureUnitUniform");;
	glUniform2fARB(textureUnitUniform,textureUnit.u,textureUnit.v);
}

ShadowShader::~ShadowShader(void)
{
}

void ShadowShader::enable()
{
	ShaderProgram::enable();
	glUniform2fARB(textureUnitUniform,textureUnit.u,textureUnit.v);
}