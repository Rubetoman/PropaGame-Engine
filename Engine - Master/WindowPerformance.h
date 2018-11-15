#pragma once

#include "Window.h"
#include <vector>

class WindowPerformance : public Window
{
public:
	WindowPerformance(const char * name);
	~WindowPerformance();

	void Draw();

	// Performance data
	std::vector<float> fps_log;
	std::vector<float> ms_log;
	std::vector<float> mem_log;
};

