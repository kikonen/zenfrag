#include "TRIncreaseExposure.h"
#include "Timer.h"

CTRIncreaseExposure::CTRIncreaseExposure(LightScatteringShader* lightScatteringShader)
{
	this->lightScatteringShader = lightScatteringShader;
}

CTRIncreaseExposure::~CTRIncreaseExposure(void)
{
}


void CTRIncreaseExposure::keyPressed()
{
	lightScatteringShader->uniformExposure += 0.01f * Timer::tick / (float)1000;
	printf("Engine::engine->uniformExposure=%f\n",lightScatteringShader->uniformExposure);
}
