#pragma once

#include "SDL.h"

class KeyboardKeyHander
{
public:
	KeyboardKeyHander(void);
	~KeyboardKeyHander(void);

	SDLKey keyMonitored;
	bool keyWasPressed;

	void setMonitoredKey(SDLKey value);
	virtual void onKeyPressed() ;
	virtual void keyPressed() ;
	virtual void onKeyReleased() ;
};
