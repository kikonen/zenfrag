#pragma once

#include "KeyboardKeyHander.h"

class CRTSwitchCamera : public KeyboardKeyHander
{
public:
	CRTSwitchCamera(void);
	~CRTSwitchCamera(void);


	virtual void onKeyPressed() ;

};
