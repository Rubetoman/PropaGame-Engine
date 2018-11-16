#ifndef __WINDOWSCENE_H__
#define __WINDOWSCENE_H__

#include "Window.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"

class WindowScene : public Window
{
public:
	WindowScene(const char* name);
	~WindowScene();
	bool IsFocused() const;

	void Draw();

	bool focus = false;
};
#endif /*__WINDOWSCENE_H__*/
