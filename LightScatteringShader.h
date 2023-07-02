#pragma once
#include "shaderprogram.h"

class Light;

class LightScatteringShader :
	public ShaderProgram
{
public:
	LightScatteringShader(char* vertexShader, char* fragmentShader,Light* light);
	~LightScatteringShader(void);
	virtual void enable();
	//virtual void bindUniformsAndAttribute(void);
	void  getLightScreenCoor();
	Light* light;

	void grabLightCoo();

	float uniformLightX ;
	float uniformLightY ;
	float uniformExposure;
	float uniformDecay;
	float uniformDensity;
	float uniformWeight;
	float prepassTexture;

private:

	
	unsigned int glsl_loc_light;
	unsigned int glsl_loc_exposure;
	unsigned int glsl_loc_decay;
	unsigned int glsl_loc_density;
	unsigned int glsl_loc_weight;
	unsigned int glsl_loc_myTexture;

};
