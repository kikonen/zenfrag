#include "TRIncreaseWeight.h"
#include "Timer.h"

CTRIncreaseWeight::CTRIncreaseWeight(LightScatteringShader* lightScatteringShader)
{
	this->lightScatteringShader = lightScatteringShader;
}

CTRIncreaseWeight::~CTRIncreaseWeight(void)
{
}

void CTRIncreaseWeight::keyPressed()
{
	lightScatteringShader->uniformWeight += 0.1f * Timer::tick / (float)1000;
	printf("Engine::engine->uniformWeight=%f\n",lightScatteringShader->uniformWeight);
}