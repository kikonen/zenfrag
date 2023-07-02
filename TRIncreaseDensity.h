#pragma once
#include "keyboardkeyhander.h"
#include "LightScatteringShader.h"

class CTRIncreaseDensity :
	public KeyboardKeyHander
{
public:
	CTRIncreaseDensity(LightScatteringShader* lightScatteringShader);
	LightScatteringShader* lightScatteringShader;
	~CTRIncreaseDensity(void);
	virtual void keyPressed() ;
};
