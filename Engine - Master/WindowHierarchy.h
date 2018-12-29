#ifndef __WINDOWHIERARCHY_H__
#define __WINDOWHIERARCHY_H__

#include "Window.h"

class GameObject;

class WindowHierarchy : public Window
{
public:
	WindowHierarchy(const char* name);
	~WindowHierarchy();

	void Draw();
	void DrawNode();
	void DrawChildren(GameObject* node);
	void SetDragAndDrop(GameObject* node);

	GameObject* selected = false;
};

#endif /*__WINDOWHIERARCHY_H__*/