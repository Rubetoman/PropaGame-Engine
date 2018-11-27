#ifndef __WINDOWHIERARCHY_H__
#define __WINDOWHIERARCHY_H__

#include "Window.h"
#include "GameObject.h"

class WindowHierarchy : public Window
{
public:
	WindowHierarchy(const char* name);
	~WindowHierarchy();

	void Draw();
	void DrawNode();
	void DrawChildren(GameObject* node);
	GameObject* selected = false;
};

#endif /*__WINDOWHIERARCHY_H__*/