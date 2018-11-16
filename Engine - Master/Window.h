#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "Globals.h"
#include "Module.h"
#include "ImGui/imgui.h"
#include <string>

class Window
{
public:
	Window(const char* name) : name(name) {}
	virtual ~Window() {}

	virtual void Draw() {}
	void toggleActive() { active = !active; }

	bool isActive() const { return active; }
	const char* getName() const { return name.c_str(); }

protected:

	std::string name;
	bool active = false;

};

#endif /*__WINDOW_H__*/