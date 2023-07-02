#include "TRDecreaseWeight.h"

#include "Timer.h"

CTRDecreaseWeight::CTRDecreaseWeight(LightScatteringShader* lightScatteringShader)
{
	this->lightScatteringShader = lightScatteringShader;
}

CTRDecreaseWeight::~CTRDecreaseWeight(void)
{
}

void CTRDecreaseWeight::keyPressed()
{
	lightScatteringShader->uniformWeight -= 0.1f * Timer::tick / (float)1000;
	printf("Engine::engine->uniformWeight=%f\n",lightScatteringShader->uniformWeight);
}
