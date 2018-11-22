#ifndef __WINDOWCONFIGURATION_H__
#define __WINDOWCONFIGURATION_H__

#include "Window.h"
#include <vector>

class WindowConfiguration : public Window
{
public:
	WindowConfiguration(const char* name);
	~WindowConfiguration();

	void Draw();

	// Game performance
	std::vector<float> fps_game_log;
	std::vector<float> ms_game_log;

	// App performance data
	std::vector<float> fps_log;
	std::vector<float> ms_log;
	std::vector<float> mem_log;
};

#endif /*__WINDOWCONFIGURATION_H__*/