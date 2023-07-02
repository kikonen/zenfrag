#pragma once
#include "keyboardkeyhander.h"
#include "LightScatteringShader.h"

class CTRDecreaseWeight :
	public KeyboardKeyHander
{
public:
	CTRDecreaseWeight(LightScatteringShader* lightScatteringShader);
	LightScatteringShader* lightScatteringShader;
	~CTRDecreaseWeight(void);
	virtual void keyPressed() ;
};
