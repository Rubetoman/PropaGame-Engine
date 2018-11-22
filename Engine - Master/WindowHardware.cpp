#include "WindowHardware.h"
#include "SDL/include/SDL.h"
#include "mmgr/mmgr.h"

WindowHardware::WindowHardware(const char* name) : Window(name)
{
}

WindowHardware::~WindowHardware()
{
}

void WindowHardware::Draw()
{
	ImGui::SetNextWindowSize(ImVec2(350, 250), ImGuiCond_FirstUseEver);
	ImGui::Begin("Hardware Info", &active, ImGuiWindowFlags_NoFocusOnAppearing);   // Pointer to bool variable (close when click on button)

	ImGui::Text("CPUs:");
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, { 1,1,0,1 });
	ImGui::Text("%d (Cache: %dB)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
	ImGui::PopStyleColor();

	ImGui::Text("System RAM:");
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, { 1,1,0,1 });
	ImGui::Text("%dGb", SDL_GetSystemRAM() / 1000);
	ImGui::PopStyleColor();

	ImGui::Text("Caps:");
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, { 1,1,0,1 });
	std::string caps;
	if (SDL_Has3DNow())
		caps += "3DNow!, ";
	if (SDL_HasAVX())
		caps += "AVX, ";
	if (SDL_HasAVX2())
		caps += "AVX2, ";
	if (SDL_HasAltiVec())
		caps += "AltiVec, ";
	if (SDL_HasMMX())
		caps += "MMX, ";
	if (SDL_HasRDTSC())
		caps += "RDTSC, ";
	if (caps.size() > 5)
		caps += "\n";
	if (SDL_HasSSE())
		caps += "SSE, ";
	if (SDL_HasSSE2())
		caps += "SSE2, ";
	if (SDL_HasSSE3())
		caps += "SSE3, ";
	if (SDL_HasSSE41())
		caps += "SSE41, ";
	if (SDL_HasSSE42())
		caps += "SSE42, ";
	ImGui::Text(caps.c_str());
	ImGui::PopStyleColor();
	ImGui::End();
}