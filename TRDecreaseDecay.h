#pragma once
#include "keyboardkeyhander.h"
#include "LightScatteringShader.h"

class CTRDecreaseDecay :
	public KeyboardKeyHander
{
public:
	CTRDecreaseDecay(LightScatteringShader* lightScatteringShader);
	~CTRDecreaseDecay(void);

	LightScatteringShader* lightScatteringShader;
	

	virtual void keyPressed() ;
};
