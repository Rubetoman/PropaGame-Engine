#ifndef __WINDOWHIERARCHY_H__
#define __WINDOWHIERARCHY_H__

#include "Window.h"

class WindowHierarchy : public Window
{
public:
	WindowHierarchy(const char* name);
	~WindowHierarchy();

	void Draw();
};

#endif /*__WINDOWHIERARCHY_H__*/