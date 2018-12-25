#ifndef __WINDOWCAMERA_H__
#define __WINDOWCAMERA_H__

#include "Window.h"

class WindowCamera : public Window
{
public:
	WindowCamera(const char* name);
	~WindowCamera();

	bool IsFocused() const;

	void Draw();

public:
	bool focus = false;
};
#endif /*__WINDOWCAMERA_H__*/