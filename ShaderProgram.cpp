#include "ShaderProgram.h"
#include "ShaderLoader.h"

ShaderProgram::ShaderProgram(char* vertexShaderSource, char* fragmentShaderSource)
{
	ShaderLoader loader;

 	vertexShader = loader.loadShader(vertexShaderSource,GL_VERTEX_SHADER);
	fragmentShader = loader.loadShader(fragmentShaderSource,GL_FRAGMENT_SHADER);

	shaderId = glCreateProgramObjectARB();
			

	glAttachObjectARB(shaderId,vertexShader);
	glAttachObjectARB(shaderId,fragmentShader);
		
	glLinkProgramARB(shaderId);
}

ShaderProgram::~ShaderProgram(void)
{
}

void ShaderProgram::enable()
{
	glUseProgramObjectARB(this->shaderId);
}

void ShaderProgram::disable()
{
	glUseProgramObjectARB(0);
}
