#pragma once

class Timer
{
public:
	Timer(void);
	~Timer(void);
	static void update(void);
	static void stop(void);
	static void resume(void);
	static int tick;
	static int frames;
	static float deltas ;
	static int lastTime ;
	static float fps;
	static int paused;
};
