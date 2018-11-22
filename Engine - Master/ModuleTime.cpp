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
	real_time_clock.Start();
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
	real_time = real_time_clock.Read_Seconds();

	real_delta_time = frame_timer.Read_Seconds(); 	// Time for one frame (Real Time)
	if (game_running == Game_State::Running)
	{
		++total_frame_count;						// Game frames count
		delta_time = frame_timer.Read_Seconds() / time_scale;	// Time for one frame (Game Clock)
		time = time_scale * game_clock.Read_Seconds();			// Time since the game started (Game Clock)
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
	game_running = Game_State::Running;
	game_clock.Start();
}

void ModuleTime::Pause_Game(bool pause)
{
	if (pause)
	{
		game_running = Game_State::Paused;
		game_clock.Pause();
		delta_time = 0.0f;
	}
	else
	{
		game_running = Game_State::Running;
		game_clock.Start();
	}
}

void ModuleTime::Stop_Game()
{
	game_running = Game_State::Stoped;
	delta_time = 0.0f;
	game_clock.Stop();
	game_clock.Reset();
	time = game_clock.Read_Seconds();
	total_frame_count = 0u;
}

void ModuleTime::Step()
{
	step_frame = true;
}
