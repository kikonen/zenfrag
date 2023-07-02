#pragma once

#include "SDL2/SDL.h"

class KeyboardKeyHander
{
public:
	KeyboardKeyHander(void);
	~KeyboardKeyHander(void);

	SDL_Keycode keyMonitored;
	bool keyWasPressed;

	void setMonitoredKey(SDL_Keycode value);
	virtual void onKeyPressed() ;
	virtual void keyPressed() ;
	virtual void onKeyReleased() ;
};
