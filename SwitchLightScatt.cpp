#include "SwitchLightScatt.h"
#include "Engine.h"

SwitchLightScatt::SwitchLightScatt(void)
{
}

SwitchLightScatt::~SwitchLightScatt(void)
{
}


void SwitchLightScatt::onKeyPressed()
{
	Engine::engine->lightScatteringActivated = !Engine::engine->lightScatteringActivated ;
		
}