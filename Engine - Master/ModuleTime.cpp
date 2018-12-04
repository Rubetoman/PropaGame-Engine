#include "ModuleTime.h"

ModuleTime::ModuleTime()
{
}

ModuleTime::~ModuleTime()
{
}

bool ModuleTime::Init()
{
	delta_time = 0.0f;
	frame_timer.Start();
	fps_timer.Start();
	return true;
}

update_status ModuleTime::Update()
{
	// Frame count
	++frame_count;
	++real_total_frame_count;

	// Time update
	real_delta_time = frame_timer.Read_Seconds(); 	// Time for one frame (Real Time)
	real_time += real_delta_time;					// Time since the App started (Real Time Clock)

	if (game_running == Game_State::Running)
	{
		++total_frame_count;						// Game frames count
		delta_time = frame_timer.Read_Seconds();	// Time for one frame (Game Clock)
		time += delta_time * time_scale;			// Time since the game started (Game Clock)
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
	frame_timer.Stop();
	fps_timer.Stop();
	return true;
}

void ModuleTime::Start_Game()
{
	game_running = Game_State::Running;
}

void ModuleTime::Pause_Game(bool pause)
{
	if (pause)
	{
		game_running = Game_State::Paused;
		delta_time = 0.0f;
	}
	else
	{
		game_running = Game_State::Running;
	}
}

void ModuleTime::Stop_Game()
{
	game_running = Game_State::Stoped;
	delta_time = 0.0f;
	time = 0.0f;
	total_frame_count = 0u;
}

void ModuleTime::Step()
{
	step_frame = true;
}
