#include "TRDecreaseDensity.h"
#include "Timer.h"

CTRDecreaseDensity::CTRDecreaseDensity(LightScatteringShader* lightScatteringShader)
{
	this->lightScatteringShader = lightScatteringShader;
}

CTRDecreaseDensity::~CTRDecreaseDensity(void)
{
}

void CTRDecreaseDensity::keyPressed()
{
	lightScatteringShader->uniformDensity -= 0.1f * Timer::tick / (float)1000;
	printf("Engine::engine->uniformDensity=%f\n",lightScatteringShader->uniformDensity);
}

