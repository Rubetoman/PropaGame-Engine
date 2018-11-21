#include "ModuleTime.h"

ModuleTime::ModuleTime()
{
}

ModuleTime::~ModuleTime()
{
}

bool ModuleTime::Init()
{
	delta_time = 0;
	real_time_clock.Start();
	frame_timer.Start();
	fps_timer.Start();
	return true;
}

update_status ModuleTime::Update()
{
	// Frame count
	++frame_count;
	++total_frame_count;

	real_delta_time = frame_timer.Read(); 	// Time for one frame (Real Time)
	if (game_running)
	{
		delta_time = frame_timer.Read();	// Time for one frame (Game Clock)
		time = game_clock.Read();			// Time since the game started (Game Clock)
	}
	frame_timer.Reset();

	// Frames per second
	if (fps_timer.Read_Seconds() >= 1.0f)
	{
		FPS = frame_count;
		frame_count = 0u;
		fps_timer.Reset();
	}
	return UPDATE_CONTINUE;
}

bool ModuleTime::CleanUp()
{
	return true;
}

void ModuleTime::Start_Game()
{
	game_running = true;
	game_clock.Start();
}

void ModuleTime::Pause_Game(bool pause)
{
	game_running = !pause;
	if (pause)
		game_clock.Pause();
	else
		game_clock.Start();
}

void ModuleTime::Stop_Game()
{
	game_running = false;
	game_clock.Stop();
}
