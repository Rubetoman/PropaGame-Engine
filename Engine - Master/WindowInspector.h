#ifndef __WINDOWINSPECTOR_H__
#define __WINDOWINSPECTOR_H__

#include "Window.h"

class GameObject;

class WindowInspector : public Window
{
public:
	WindowInspector(const char* name);
	~WindowInspector();

	void Draw();
	void DrawComponents(GameObject* go);
};

#endif /*__WINDOWINSPECTOR_H__*/