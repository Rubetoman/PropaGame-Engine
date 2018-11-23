#include "WindowInspector.h"



WindowInspector::WindowInspector(const char* name) : Window(name)
{
	active = true;
}


WindowInspector::~WindowInspector()
{
}

void WindowInspector::Draw()
{
	if (ImGui::Begin("Inspector"))
	{
	}
	ImGui::End();
}