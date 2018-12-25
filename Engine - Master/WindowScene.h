#ifndef __WINDOWSCENE_H__
#define __WINDOWSCENE_H__

#include "Window.h"

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
