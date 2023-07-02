#pragma once

#include "glex_impl.h"

class ShaderProgram
{
public:
	ShaderProgram(char* vertexShader, char* fragmentShader);
	~ShaderProgram(void);

	virtual void enable();
	void disable();

	GLhandleARB shaderId;

protected:

	GLhandleARB vertexShader;
	GLhandleARB fragmentShader;
	

};
