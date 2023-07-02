#include "LightScatteringShader.h"
#include "Light.h"
#include "main.h"
#include "Engine.h"

LightScatteringShader::LightScatteringShader(char* vertexShader, char* fragmentShader, Light* light) : ShaderProgram(vertexShader,fragmentShader)
{
	uniformExposure = 0.001300f;
	uniformDecay = 1.006500f;
	uniformDensity = 0.476197f;
	uniformWeight = 20.013634f;	
	
		
	this->light = light;

	//Set light screen coordinate
	//lightPositionOnScreen
	

	glsl_loc_light = glGetUniformLocationARB(shaderId,"lightPositionOnScreen");
	glUniform2fARB(glsl_loc_light,uniformLightX,uniformLightY);

	glsl_loc_exposure = glGetUniformLocationARB(shaderId,"exposure");
	glUniform1fARB(glsl_loc_exposure,uniformExposure);
		
	glsl_loc_decay = glGetUniformLocationARB(shaderId,"decay");
	glUniform1fARB(glsl_loc_decay,uniformDecay);

	glsl_loc_density = glGetUniformLocationARB(shaderId,"density");
	glUniform1fARB(glsl_loc_density,uniformDensity);

	glsl_loc_weight = glGetUniformLocationARB(shaderId,"weight");
	glUniform1fARB(glsl_loc_weight,uniformWeight);

	glsl_loc_myTexture = glGetUniformLocationARB(shaderId,"myTexture");
	glUniform1iARB(glsl_loc_myTexture,0);

	getLightScreenCoor();
	glUniform2fARB(glsl_loc_light,uniformLightX,uniformLightY);
}

LightScatteringShader::~LightScatteringShader(void)
{
}

void LightScatteringShader::grabLightCoo()
{
	getLightScreenCoor();
}

void LightScatteringShader::enable()
{
	ShaderProgram::enable();

	glUniform1fARB(glsl_loc_exposure,uniformExposure);
	glUniform1fARB(glsl_loc_decay,uniformDecay);
	glUniform1fARB(glsl_loc_density,uniformDensity);
	glUniform1fARB(glsl_loc_weight,uniformWeight);
	glUniform1iARB(glsl_loc_myTexture,0);

	
	glUniform2fARB(glsl_loc_light,uniformLightX,uniformLightY);
}




void LightScatteringShader::getLightScreenCoor()
{
	double modelView[16];
    double projection[16];
    GLint viewport[4];
    double depthRange[2];

	glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_DEPTH_RANGE, depthRange);

	GLdouble winX=0;
	GLdouble winY=0;
	GLdouble winZ=0;

	gluProject(	this->light->position.x,
				this->light->position.y,
				this->light->position.z,
				modelView,
				projection,
				viewport,
				&winX,
				&winY,
				&winZ);
	
	//printf("winX=%f,winY=%f\n",winX,winY);

	uniformLightX = winX/((float)Engine::engine->renderWidth);///LIGHT_SCAT_RENDER_RATIO);
	uniformLightY = winY/((float)Engine::engine->renderHeight);///LIGHT_SCAT_RENDER_RATIO) ;


}