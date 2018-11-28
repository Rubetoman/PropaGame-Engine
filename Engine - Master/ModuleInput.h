#ifndef __MODULEINPUT_H__
#define __MODULEINPUT_H__

#include "Module.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ModuleFileManager.h"
#include "SDL/include/SDL.h"
#include "SDL_scancode.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "Point.h"

#define NUM_MOUSE_BUTTONS 5

enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status Update();
	bool CleanUp();

	// Check key states (includes mouse and joy buttons)
	inline KeyState GetKey(const int id) const
	{
		return keyboard[id];
	}

	inline KeyState GetMouseButtonDown(const int id) const
	{
		return mouse_buttons[id - 1];
	}

	// Check for window events last frame
	bool GetWindowEvent(EventWindow code) const;

	// Mouse Input
	const fPoint& GetMouseMotion() const;	
	const fPoint& GetMousePosition() const;		
	const fPoint& GetMouseWheel() const;		

private:
	bool		windowEvents[WE_COUNT];
	KeyState*	keyboard;
	KeyState	mouse_buttons[NUM_MOUSE_BUTTONS];
	fPoint		mouse_motion;
	fPoint		mouse;
	fPoint		wheel_motion;
	bool		wheel_scroll;

	char* dropped_filedir = nullptr;    // Pointer for directory of dropped file
};

#endif /*__MODULEINPUT_H__*/