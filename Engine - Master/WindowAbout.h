#ifndef __WINDOWABOUT_H__
#define __WINDOWABOUT_H__

#include "Window.h"

class WindowAbout : public Window
{
public:
	WindowAbout(const char* name);
	~WindowAbout();

	void Draw();
};

#endif /*__WINDOWABOUT_H__*/
