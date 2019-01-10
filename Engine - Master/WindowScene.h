#ifndef __WINDOWSCENE_H__
#define __WINDOWSCENE_H__

#include "Window.h"
#include "Math/float2.h"

class WindowScene : public Window
{
public:
	WindowScene(const char* name);
	~WindowScene();

	void Draw() override;
	bool IsFocused() const;

public:
	bool focus = false;
	math::float2 viewport = math::float2::zero;

	// Guizmo
	bool gui_click = false;			// True if is hovering a GUI item of WindowScene
};
#endif /*__WINDOWSCENE_H__*/
