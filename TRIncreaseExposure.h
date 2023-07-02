#pragma once
#include "keyboardkeyhander.h"
#include "LightScatteringShader.h"

class CTRIncreaseExposure : public KeyboardKeyHander
{
public:
	CTRIncreaseExposure(LightScatteringShader* lightScatteringShader);
	~CTRIncreaseExposure(void);
	
	LightScatteringShader* lightScatteringShader;

	virtual void keyPressed() ;
};
