#include "Pause.h"
#include "Timer.h"

Pause::Pause(void)
{
}

Pause::~Pause(void)
{
}

void Pause::onKeyPressed()
{

		if (Timer::paused)
			Timer::resume();
		else
			Timer::stop();
		 
	
	
}
