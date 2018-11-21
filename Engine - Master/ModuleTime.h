#ifndef __MODULETIME_H__
#define __MODULETIME_H__

#include "Application.h"
#include "Timer.h"
#include "SDL.h"

class ModuleTime : public Module
{
public:
	ModuleTime();
	~ModuleTime();

	bool Init();
	update_status Update();
	bool CleanUp();

	void Start_Game();
	void Pause_Game();
	void Stop_Game();

public:

	unsigned total_frame_count = 0u;	// App graphics frames since game start
	float time = 0.0f;					// Second since game start (Game Clock)
	float time_scale = 0.0f;			// Scale at which game time is passing
	float delta_time = 0.0f;			// Miliseconds for one frame
	int FPS = 0;						// Number of frames passed in one second

	// Real Time Clock
	float real_time = 0.0f;
	float real_delta_time = 0.0f;

	// Timers
	Timer game_clock;		// Game Clock
	Timer real_time_clock;	// Real Time Clock
	Timer frame_timer;		// Timer reseted each frame
	Timer fps_timer;		// Timer reseted each second

	bool game_running = false;	// Is the game running?
	
private:
	unsigned frame_count = 0u;			// Frames counter
};

#endif /*__MODULETIME_H__*/