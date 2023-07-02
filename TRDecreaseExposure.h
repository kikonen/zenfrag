#pragma once
#include "keyboardkeyhander.h"
#include "LightScatteringShader.h"

class CTRDecreaseExposure :
	public KeyboardKeyHander
{
public:
	CTRDecreaseExposure(LightScatteringShader* lightScatteringShader);
	~CTRDecreaseExposure(void);
	LightScatteringShader* lightScatteringShader;
	virtual void keyPressed() ;
};
