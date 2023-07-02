#pragma once
#include "keyboardkeyhander.h"
#include "LightScatteringShader.h"

class CTRIncreaseWeight :
	public KeyboardKeyHander
{
public:
	CTRIncreaseWeight(LightScatteringShader* lightScatteringShader);
	LightScatteringShader* lightScatteringShader;
	~CTRIncreaseWeight(void);
	virtual void keyPressed() ;
};
