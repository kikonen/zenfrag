#pragma once
#include "ShadowShader.h"
#include "md5.h"

class CreatureShader : public ShadowShader
{
public:
	CreatureShader(char* vertexShader, char* fragmentShader);
	~CreatureShader(void);
	virtual void enable();
	//virtual void bindUniformsAndAttribute(void);
	
	void setCurrentModel(md5Object* object);

	GLint tangentLoc;

	
private:

	GLint shadowMapUniform;
	GLint shadowMapBackUniform;

	GLint diffuseTextureUniform;
	GLint specularTextureUniform;
	GLint normalTextureUniform;
	
};
