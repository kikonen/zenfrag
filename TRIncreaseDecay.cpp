#include "TRIncreaseDecay.h"

#include "Timer.h"

CTRIncreaseDecay::CTRIncreaseDecay(LightScatteringShader* lightScatteringShader)
{
	this->lightScatteringShader = lightScatteringShader;
}

CTRIncreaseDecay::~CTRIncreaseDecay(void)
{
}

void CTRIncreaseDecay::keyPressed()
{
	lightScatteringShader->uniformDecay += 0.1f * Timer::tick / (float)1000;
	printf("Engine::engine->uniformDecay=%f\n",lightScatteringShader->uniformDecay);
}