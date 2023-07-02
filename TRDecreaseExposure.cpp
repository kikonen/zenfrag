#include "TRDecreaseExposure.h"
#include "Timer.h"

CTRDecreaseExposure::CTRDecreaseExposure(LightScatteringShader* lightScatteringShader)
{
	this->lightScatteringShader = lightScatteringShader;
}

CTRDecreaseExposure::~CTRDecreaseExposure(void)
{
}

void CTRDecreaseExposure::keyPressed()
{
	lightScatteringShader->uniformExposure -= 0.01f * Timer::tick / (float)1000;
	printf("Engine::engine->uniformExposure=%f\n",lightScatteringShader->uniformExposure);
}

