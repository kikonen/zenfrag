#pragma once
#include "keyboardkeyhander.h"

class Pause :
	public KeyboardKeyHander
{
public:
	Pause(void);
	~Pause(void);

	void onKeyPressed();
};
