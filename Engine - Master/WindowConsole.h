#ifndef __WINDOWCONSOLE_H__
#define __WINDOWCONSOLE_H__

#include "Window.h"

class WindowConsole : public Window
{
public:
	WindowConsole(const char* name);
	~WindowConsole();

	void Draw();
	void AddLog(const char* logs);
	void Clear() { Buffer.clear(); }

	ImGuiTextBuffer Buffer;
	bool ScrollToBottom = false;
};

#endif /*__WINDOWCONSOLE_H__*/


