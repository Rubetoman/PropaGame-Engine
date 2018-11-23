#ifndef __WINDOWINSPECTOR_H__
#define __WINDOWINSPECTOR_H__

#include "Window.h"

class WindowInspector : public Window
{
public:
	WindowInspector(const char* name);
	~WindowInspector();

	void Draw();
};

#endif /*__WINDOWINSPECTOR_H__*/