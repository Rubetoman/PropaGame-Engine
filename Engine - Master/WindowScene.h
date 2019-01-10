#ifndef __WINDOWSCENE_H__
#define __WINDOWSCENE_H__

#include "Window.h"
#include "Math/float2.h"

class WindowScene : public Window
{
public:
	WindowScene(const char* name);
	~WindowScene();
	bool IsFocused() const;

	void Draw();

	bool focus = false;
	math::float2 viewport = math::float2::zero;
	bool gui_click = false;			// True if is hovering a GUI item of WindowScene
};
#endif /*__WINDOWSCENE_H__*/
