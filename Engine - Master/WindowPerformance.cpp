#include "WindowPerformance.h"
#include "SDL/include/SDL.h"
#include "mmgr/mmgr.h"

WindowPerformance::WindowPerformance(const char* name) : Window(name)
{
	fps_log.resize(100);
	ms_log.resize(100);
	mem_log.resize(100);
}


WindowPerformance::~WindowPerformance()
{
}

void WindowPerformance::Draw()
{
	ImGui::SetNextWindowSize(ImVec2(350, 500), ImGuiCond_FirstUseEver);
	ImGui::Begin("Performance", &active, ImGuiWindowFlags_NoFocusOnAppearing);   // Pointer to bool variable (close when click on button)

	// FPS & Miliseconds for each frame
	char title[35];
	sprintf_s(title, 25, "Framerate %0.1f", fps_log[fps_log.size() - 1]);
	ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 200.0f, ImVec2(310, 100));
	sprintf_s(title, 25, "Milliseconds %0.1f", ms_log[ms_log.size() - 1]);
	ImGui::PlotHistogram("##framerate", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));

	// Memory stats
	sMStats stats = m_getMemoryStatistics();
	mem_log.erase(mem_log.begin());
	mem_log.push_back((float)stats.totalReportedMemory);
	sprintf_s(title, 35, "Memory Consumption (Bytes) %0.1f", mem_log[mem_log.size() - 1]);
	ImGui::PlotHistogram("##memory", &mem_log[0], mem_log.size(), 0, title, 0.0f, (float)stats.peakReportedMemory * 1.2f, ImVec2(310, 100));
	ImGui::Text("Total Reported Mem: %u", stats.totalReportedMemory);
	ImGui::Text("Total Actual Mem: %u", stats.totalActualMemory);
	ImGui::Text("Peak Reported Mem: %u", stats.peakReportedMemory);
	ImGui::Text("Peak Actual Mem: %u", stats.peakActualMemory);
	ImGui::Text("Accumulated Reported Mem: %u", stats.accumulatedReportedMemory);
	ImGui::Text("Accumulated Actual Mem: %u", stats.accumulatedActualMemory);
	ImGui::Text("Accumulated Alloc Unit Count: %u", stats.accumulatedAllocUnitCount);
	ImGui::Text("Total Alloc Unit Count: %u", stats.totalAllocUnitCount);
	ImGui::Text("Peak Alloc Unit Count: %u", stats.peakAllocUnitCount);

	if (ImGui::CollapsingHeader("Hardware"))
	{
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
	}
	ImGui::End();
}