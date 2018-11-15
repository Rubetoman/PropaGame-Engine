#include "WindowConsole.h"

WindowConsole::WindowConsole(const char* name) : Window(name)
{
	active = true;
}

WindowConsole::~WindowConsole()
{
}

void WindowConsole::Draw() 
{
	ImGui::Begin("Console", &active, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_MenuBar);   // Pointer to bool variable (close when click on button)
	ImGui::BeginMenuBar();
	ImGui::PushStyleColor(ImGuiCol_Button, { 0.9f,0.45f,0.0f,0.7f });
	if (ImGui::Button("Clear"))
	{
		Buffer.clear();
	}
	ImGui::PopStyleColor();
	ImGui::EndMenuBar();

	ImGui::TextUnformatted(Buffer.begin());

	if (ScrollToBottom)
		ImGui::SetScrollHere(1.0f);

	ScrollToBottom = false;

	ImGui::End();
}

void WindowConsole::AddLog(const char* logs)
{
	assert(logs != nullptr);
	Buffer.appendf(logs);
	ScrollToBottom = true;
}
