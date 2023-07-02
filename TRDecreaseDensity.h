#pragma once
#include "keyboardkeyhander.h"
#include "LightScatteringShader.h"

class CTRDecreaseDensity :
	public KeyboardKeyHander
{
public:
	CTRDecreaseDensity(LightScatteringShader* lightScatteringShader);
	~CTRDecreaseDensity(void);
	LightScatteringShader* lightScatteringShader;
	virtual void keyPressed() ;
};
