#include "CreatureShader.h"


CreatureShader::CreatureShader(char* vertexShader, char* fragmentShader) : ShadowShader(vertexShader,fragmentShader)
{
	shadowMapUniform = glGetUniformLocationARB(shaderId,"ShadowMap");
	shadowMapBackUniform = glGetUniformLocationARB(shaderId,"BackShadowMap");

	diffuseTextureUniform = glGetUniformLocationARB(shaderId,"diffuseTexture");
	specularTextureUniform = glGetUniformLocationARB(shaderId,"specularTexture");
	normalTextureUniform = glGetUniformLocationARB(shaderId,"normalTexture");

	tangentLoc = glGetAttribLocationARB(shaderId, "tangent");
}

CreatureShader::~CreatureShader(void)
{
}

void CreatureShader::enable()
{
	ShadowShader::enable();
	
	glUniform1iARB(diffuseTextureUniform,1);
	glUniform1iARB(specularTextureUniform,2);
	glUniform1iARB(normalTextureUniform,3);
	glUniform1iARB(shadowMapUniform,0);
	glUniform1iARB(shadowMapBackUniform,7);

}


void CreatureShader::setCurrentModel(md5Object* object)
{
	
	glEnableVertexAttribArrayARB (tangentLoc);
	glVertexAttribPointerARB (tangentLoc, 
								3, 
								GL_FLOAT, 
								GL_FALSE,
								0, 
								object->tangentArraySkin[(int)object->animCursor]);
								
}

