#include "Timer.h"
#include "SDL2/SDL.h"

int Timer::tick = 0 ;

int Timer::frames;
float Timer::deltas ;
int Timer::lastTime ;
float Timer::fps ;
int Timer::paused ;

Timer::Timer(void)
{
	frames = 0;
	deltas = 0;
	lastTime = 0;
	fps = 0;
	paused = 0;
}

Timer::~Timer(void)
{
}

void Timer::stop(void)
{
	paused = 1;
}

void Timer::resume(void)
{
	paused = 0;
	lastTime = SDL_GetTicks();
	deltas=0;
	 frames = 0;
}

void Timer::update(void)
{
		int currentTime = SDL_GetTicks();

		//printf("pausd=%d",paused);
		//if (paused)
		//	tick = 0;
		//else
			tick =  currentTime - lastTime ;

		deltas+= tick;
        lastTime = currentTime;

//		printf("1. %d\n",SDL_GetTicks());


		frames++;
        if (deltas > 300)
        {
            fps = frames*1000/deltas;
            frames = 0;
            deltas = 0 ;
        }
}