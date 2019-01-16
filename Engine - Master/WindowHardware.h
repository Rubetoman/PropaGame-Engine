#ifndef __WINDOWHARDWARE_H__
#define __WINDOWHARDWARE_H__

#include "Window.h"

class WindowHardware : public Window
{
public:
	WindowHardware(const char* name);
	~WindowHardware();

	void Draw();
};

#endif /*__WINDOWHARDWARE_H__*/