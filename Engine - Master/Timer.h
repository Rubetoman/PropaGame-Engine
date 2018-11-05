#pragma once
#include "SDL.h"
class MsTimer 
{
	Uint32 startTicks = 0;		// Ticks number at timer start
	Uint32 lastTickCount = 0;	// How many ticks passed last time it was checked
	Uint32 time = 0;		// Time in milliseconds

	
public:
	void Start() 
	{
		startTicks = SDL_GetTicks();
	}

	Uint32 Read()
	{
		return (SDL_GetTicks() - startTicks);
	}

	void Stop()
	{
		time = (SDL_GetTicks() - startTicks);
	}

	void Reset()
	{
		time = 0;
	}
};

class McTimer
{
private:
	Uint64 startTicks = 0;		// Ticks number at timer start
	Uint64 lastTickCount = 0;	// How many ticks passed last time it was checked
	Uint64 time = 0;			// Time in milliseconds
	static Uint64 frequency;

public:
	void Start()
	{
		startTicks = SDL_GetPerformanceCounter();
	}

	inline Uint64 Read()
	{
		return (SDL_GetPerformanceCounter() - startTicks) * 1000/ frequency;
	}

	void Stop()
	{
		time = (SDL_GetPerformanceCounter() - startTicks) * 1000/ frequency;
	}

	void Reset()
	{
		time = 0;
	}
};

Uint64 McTimer::frequency = SDL_GetPerformanceFrequency();

