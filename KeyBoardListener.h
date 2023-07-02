#pragma once

#include "KeyboardKeyHander.h"
#include <vector>
using namespace std;

class KeyBoardListener
{
public:
	KeyBoardListener(void);
	~KeyBoardListener(void);

	void addListener(SDLKey key, KeyboardKeyHander* listener);

	void update(Uint8 * tKeys);

	vector<KeyboardKeyHander*> handlers;
};
