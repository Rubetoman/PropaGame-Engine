#ifndef __TIMER_H__
#define __TIMER_H__

#include "SDL.h"

class Timer
{
public:
	bool running = false;

private:
	// Milliseconds
	Uint32 start_ticks = 0;		// Ticks number at timer start
	Uint32 time = 0;			// Time in milliseconds
	Uint32 skiped_time = 0;		

	// Microseconds
	Uint32 start_ticks_mc = 0;	// Ticks number at timer start
	Uint32 time_mc = 0;			// Time in microseconds
	Uint32 skiped_time_mc = 0;			
	const Uint64 frequency = SDL_GetPerformanceFrequency();

public:
	Timer() {};
	~Timer() {};

	inline void Start()
	{
		start_ticks = SDL_GetTicks();
		start_ticks_mc = SDL_GetPerformanceCounter();
		running = true;
	}

	inline Uint32 Read()
	{
		if (running)
			time = (SDL_GetTicks() - start_ticks + skiped_time);
		return time;
	}

	inline float Read_Seconds()
	{
		if(running)
			time = (SDL_GetTicks() - start_ticks + skiped_time) / 1000.0f;
		return time;
	}

	inline Uint32 Read_Mc()
	{
		if(running)
			time = (SDL_GetPerformanceCounter() - start_ticks_mc + skiped_time_mc) * 1000 / frequency;
		return time;
	}

	inline void Pause()
	{
		skiped_time += (SDL_GetTicks() - start_ticks);
		skiped_time_mc += (SDL_GetPerformanceCounter() - start_ticks_mc) * 1000 / frequency;
		running = false;
	}

	inline void Stop()
	{
		running = false;
	}

	inline void Reset()
	{
		start_ticks = SDL_GetTicks();
		skiped_time = 0;
		skiped_time_mc = 0;
	}
};

#endif // __TIMER_H__