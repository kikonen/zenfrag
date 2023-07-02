#include "KeyBoardListener.h"
#include "KeyboardKeyHander.h"

KeyBoardListener::KeyBoardListener(void)
{
}

KeyBoardListener::~KeyBoardListener(void)
{
}

void KeyBoardListener::addListener(SDL_Keycode key, KeyboardKeyHander* listener)
{
	listener->setMonitoredKey(key);
	handlers.push_back(listener);
}

void KeyBoardListener::update(const Uint8 * tKeys)
{
	KeyboardKeyHander* handler = NULL;
	for( unsigned int i=0 ; i < handlers.size() ; i++)
	{
		handler = handlers.at(i);
		if (tKeys[handler->keyMonitored])
		{
			if (handler->keyWasPressed)
				handler->keyPressed();
			else
				handler->onKeyPressed();

			handler->keyWasPressed = true;
		}
		else
			handler->keyWasPressed = false;
	}
}
