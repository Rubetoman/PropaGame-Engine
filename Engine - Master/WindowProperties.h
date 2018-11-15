#ifndef __WINDOWPROPERTIES_H__
#define __WINDOWPROPERTIES_H__

#include "Window.h"

class WindowProperties : public Window
{
public:
	WindowProperties(const char* name);
	~WindowProperties();

	void Draw();
};

#endif /*__WINDOWPROPERTIES_H__*/