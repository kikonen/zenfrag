#pragma once

#include "KeyboardKeyHander.h"
#include <vector>
using namespace std;

class KeyBoardListener
{
public:
	KeyBoardListener(void);
	~KeyBoardListener(void);

	void addListener(SDL_Keycode key, KeyboardKeyHander* listener);

	void update(const Uint8 * tKeys);

	vector<KeyboardKeyHander*> handlers;
};
