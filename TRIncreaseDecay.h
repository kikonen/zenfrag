#pragma once
#include "keyboardkeyhander.h"
#include "LightScatteringShader.h"

class CTRIncreaseDecay :
	public KeyboardKeyHander
{
public:
	CTRIncreaseDecay(LightScatteringShader* lightScatteringShader);
	~CTRIncreaseDecay(void);

	LightScatteringShader* lightScatteringShader;

	virtual void keyPressed() ;
};
