#include "KeyboardKeyHander.h"

KeyboardKeyHander::KeyboardKeyHander(void)
{
}

KeyboardKeyHander::~KeyboardKeyHander(void)
{
}

void KeyboardKeyHander::setMonitoredKey(SDL_Keycode value)
{
	this->keyMonitored = value;
}

void KeyboardKeyHander::onKeyPressed() {}
void KeyboardKeyHander::keyPressed(){}
void KeyboardKeyHander::onKeyReleased() {}
