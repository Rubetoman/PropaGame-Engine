#ifndef __WINDOWCONFIGURATION_H__
#define __WINDOWCONFIGURATION_H__

#include "Window.h"
#include <vector>

class WindowConfiguration : public Window
{
public:
	WindowConfiguration(const char* name);
	~WindowConfiguration();

	void Draw();

};

#endif /*__WINDOWCONFIGURATION_H__*/