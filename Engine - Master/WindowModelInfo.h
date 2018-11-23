#ifndef __WINDOWPROPERTIES_H__
#define __WINDOWPROPERTIES_H__

#include "Window.h"

class WindowModelInfo : public Window
{
public:
	WindowModelInfo(const char* name);
	~WindowModelInfo();

	void Draw();
};

#endif /*__WINDOWPROPERTIES_H__*/