#include "TRDecreaseDecay.h"
#include "Timer.h"

CTRDecreaseDecay::CTRDecreaseDecay(LightScatteringShader* lightScatteringShader)
{
	this->lightScatteringShader = lightScatteringShader;
}

CTRDecreaseDecay::~CTRDecreaseDecay(void)
{
}

void CTRDecreaseDecay::keyPressed()
{
	lightScatteringShader->uniformDecay -= 0.1f * Timer::tick / (float)1000;
	printf("Engine::engine->uniformDecay=%f\n",lightScatteringShader->uniformDecay);
}

