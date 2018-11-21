#ifndef __MODULETIME_H__
#define __MODULETIME_H__

#include "Application.h"
#include "Timer.h"
#include "SDL.h"

enum class Game_State{ Running, Paused, Stoped };

class ModuleTime : public Module
{
public:
	ModuleTime();
	~ModuleTime();

	bool Init();
	update_status Update();
	bool CleanUp();

	void Start_Game();
	void Pause_Game(bool pause);
	void Stop_Game();
	void Step();

public:

	unsigned total_frame_count = 0u;	// App graphics frames since game start
	float time = 0.0f;					// Second since game start (Game Clock)
	float time_scale = 0.0f;			// Scale at which game time is passing
	float delta_time = 0.0f;			// Miliseconds for one frame
	int FPS = 0;						// Number of frames passed in one second

	// Real Time Clock
	unsigned real_total_frame_count = 0u;	// App graphics frames since game start
	float real_time = 0.0f;
	float real_delta_time = 0.0f;

	// Timers
	Timer game_clock;		// Game Clock
	Timer real_time_clock;	// Real Time Clock
	Timer frame_timer;		// Timer reseted each frame
	Timer fps_timer;		// Timer reseted each second

	Game_State game_running = Game_State::Stoped;	// Is the game running?
	bool step_frame = false;	// If the clock is to be paused again after one step
	
private:
	unsigned frame_count = 0u;	// Frames counter
};

#endif /*__MODULETIME_H__*/