#pragma once
#include "keyboardkeyhander.h"

class SwitchLightScatt :
	public KeyboardKeyHander
{
public:
	SwitchLightScatt(void);
	~SwitchLightScatt(void);
	virtual void onKeyPressed() ;
};
