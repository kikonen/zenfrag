#include "TRIncreaseDensity.h"
#include "Timer.h"

CTRIncreaseDensity::CTRIncreaseDensity(LightScatteringShader* lightScatteringShader)
{
	this->lightScatteringShader = lightScatteringShader;
}

CTRIncreaseDensity::~CTRIncreaseDensity(void)
{
}

void CTRIncreaseDensity::keyPressed()
{
	lightScatteringShader->uniformDensity += 0.1f * Timer::tick / (float)1000;
	printf("Engine::engine->uniformDensity=%f\n",lightScatteringShader->uniformDensity);
}
