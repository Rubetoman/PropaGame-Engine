#include "WindowConfiguration.h"

#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleTime.h"
#include "ModuleWindow.h"
#include "ModuleScene.h"
#include "ModuleResources.h"
#include "ModuleInput.h"
#include "ModuleCamera.h"
#include "ModuleRender.h"

#include "ComponentTransform.h"
#include "ComponentCamera.h"

#include "GameObject.h"
#include "Quadtree.h"
#include "mmgr/mmgr.h"

WindowConfiguration::WindowConfiguration(const char* name) : Window(name)
{
	active = true;

	// Set histograms size
	fps_game_log.resize(50);
	ms_game_log.resize(100);

	fps_log.resize(100);
	ms_log.resize(100);
	mem_log.resize(100);
}

WindowConfiguration::~WindowConfiguration()
{
	// Clear histograms buffers
	fps_game_log.clear();
	ms_game_log.clear();

	fps_log.clear();
	ms_log.clear();
	mem_log.clear();
}

void WindowConfiguration::Draw()
{
	ImGui::SetNextWindowSize(ImVec2(350, 500), ImGuiCond_FirstUseEver);
	ImGui::Begin("Configuration", &active, ImGuiWindowFlags_NoFocusOnAppearing);   // Pointer to bool variable (close when click on button)
	if (ImGui::CollapsingHeader("Window"))
	{
		//Window size fields (only be able to edit if not in full screen)
		static ImU32 width_step = (ImU32)30;
		static ImU32 height_step = (ImU32)50;
		if (ImGui::InputScalar("Width", ImGuiDataType_U32, &App->window->screen_width, App->window->fullscreen ? NULL : &width_step, NULL, "%u"))
		{
			App->window->SetWindowSize(App->window->screen_width, App->window->screen_height, true);
		}
		if (ImGui::InputScalar("Height", ImGuiDataType_U32, &App->window->screen_height, App->window->fullscreen ? NULL : &height_step, NULL, "%u"))
		{
			App->window->SetWindowSize(App->window->screen_width, App->window->screen_height, true);
		}

		if (ImGui::Checkbox("Fullscreen", &App->window->fullscreen))
			App->window->ToggleFullScreen();
		ImGui::SameLine(110);
		if (ImGui::Checkbox("Resizable", &App->window->resizable))
			App->window->ToggleResizable();
		if (ImGui::Checkbox("VSync", &App->window->vsync))
			App->window->ToggleVSync();
		ImGui::SameLine(110);
		if (ImGui::Checkbox("Borderless", &App->window->borderless))
			App->window->ToggleBorderless();
		ImGui::Separator();

		if (ImGui::SliderFloat("Brightness", &App->window->brightness, 0.0f, 1.0f))
		{
			App->window->SetWindowBrightness(App->window->brightness);
		}
		ImGui::NewLine();
	}
	if (ImGui::CollapsingHeader("Editor"))
	{
		ImGui::Checkbox("Show hided GOs", &App->scene->show_scene_gos);
		ImGui::Checkbox("Wireframe", &App->renderer->wireframe);
		ImGui::Separator();
		// Scale to use (default: meters)
		if (ImGui::InputFloat("Scale", &App->editor->scale, 1.0f))
		{
			if (App->editor->scale < 0.001f)	// Avoid reaching 0 (It would crash)
				App->editor->scale = 0.001f;
		}
		ImGui::Checkbox("Show grid", &App->editor->show_grid); ImGui::SameLine();
		ImGui::Checkbox("Show axis", &App->editor->show_axis);
		ImGui::Separator();
		ImGui::Checkbox("Show click raycast", &App->editor->show_raycast);
		ImGui::ColorEdit3("Raycast color", (float*)&App->editor->raycast_color);
		ImGui::Separator();
		const char* modes[] = {"Selected mesh only", "BBox for each mesh on scene", "Selected GO enclosing children", "Enclose all"};
		ImGui::Combo("BBox draw mode", (int*)&App->editor->bbox_mode, modes, IM_ARRAYSIZE(modes));
	}
	if (ImGui::CollapsingHeader("Quadtree"))
	{
		ImGui::Checkbox("Use Quadtree", &App->scene->use_quadtree);
		ImGui::Checkbox("Draw Quadtree", &App->scene->draw_quadtree);
		ImGui::Separator();
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.4f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.4f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.4f, 0.8f, 0.8f));
		if (ImGui::Button("Generate Quadtree"))
		{
			App->scene->ComputeSceneQuadtree();
		}
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(1.0f, 0.8f, 0.8f));
		if (ImGui::Button("Delete Quadtree"))
		{
			App->scene->quadtree->Clear();
		}
		ImGui::PopStyleColor(3);
	}
	if (ImGui::CollapsingHeader("Editor Camera"))
	{
		ComponentCamera* camera = App->camera->editor_camera_comp;
		GameObject* camera_go = camera->my_go;

		ImGui::DragFloat3("Position", (float*)&camera_go->transform->position, 0.1f);
		//TODO: Add rotation

		ImGui::Separator();

		ImGui::Checkbox("Frustum Culling", &camera->frustum_culling);
		// View mode
		const char* items[] = { "Invalid", "Orthographic", "Perspective" };
		ImGui::Combo("View type", (int*)&camera->frustum.type, items, IM_ARRAYSIZE(items));

		ImGui::SliderFloat("Near Plane Distance", &camera->frustum.nearPlaneDistance, 0.0f, camera->frustum.farPlaneDistance);
		ImGui::SliderFloat("Far Plane Distance", &camera->frustum.farPlaneDistance, camera->frustum.nearPlaneDistance, 3000.0f);
		//ImGui::SliderFloat("Aspect Ratio", &frustum.AspectRatio(), 0, 179);
		ImGui::SliderFloat("FOV", &camera->frustum.horizontalFov, 0.0f, 3.14f);

		ImGui::Separator();

		ImGui::PushItemWidth(100.0f);
		ImGui::InputFloat("Camera Speed", &camera->speed);
		ImGui::InputFloat("Mouse Sensitivity", &App->camera->mouse_sensitivity);
		ImGui::PopItemWidth();
	}

	if (ImGui::CollapsingHeader("Performance"))
	{
		static int fps = App->time->max_fps;
		if (ImGui::SliderInt("MaxFPS", &fps, 1, 144))
			App->time->max_fps = fps;

		// FPS & Miliseconds for each frame
		char title[35];
		sprintf_s(title, 25, "Framerate %0.1f", fps_log[fps_log.size() - 1]);
		ImGui::PlotLines("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 200.0f, ImVec2(300, 50));
		sprintf_s(title, 25, "Milliseconds %0.1f", ms_log[ms_log.size() - 1]);
		ImGui::PlotLines("##framerate", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(300, 50));

		// Memory stats
		if (ImGui::TreeNode("Memory Stats"))
		{
			sMStats stats = m_getMemoryStatistics();
			mem_log.erase(mem_log.begin());
			mem_log.push_back((float)stats.totalReportedMemory);
			sprintf_s(title, 35, "Memory Consumption (Bytes) %0.1f", mem_log[mem_log.size() - 1]);
			ImGui::PlotLines("##memory", &mem_log[0], mem_log.size(), 0, title, 0.0f, (float)stats.peakReportedMemory * 1.2f, ImVec2(300, 50));
			ImGui::Text("Total Reported Mem: %u", stats.totalReportedMemory);
			ImGui::Text("Total Actual Mem: %u", stats.totalActualMemory);
			ImGui::Text("Peak Reported Mem: %u", stats.peakReportedMemory);
			ImGui::Text("Peak Actual Mem: %u", stats.peakActualMemory);
			ImGui::Text("Accumulated Reported Mem: %u", stats.accumulatedReportedMemory);
			ImGui::Text("Accumulated Actual Mem: %u", stats.accumulatedActualMemory);
			ImGui::Text("Accumulated Alloc Unit Count: %u", stats.accumulatedAllocUnitCount);
			ImGui::Text("Total Alloc Unit Count: %u", stats.totalAllocUnitCount);
			ImGui::Text("Peak Alloc Unit Count: %u", stats.peakAllocUnitCount);
			ImGui::TreePop();
		}	
	}

	if (ImGui::CollapsingHeader("Time"))
	{
		// FPS & Miliseconds for each frame
		char title[35];
		sprintf_s(title, 25, "Framerate %0.1f", fps_game_log[fps_game_log.size() - 1]);
		ImGui::PlotLines("##framerate", &fps_game_log[0], fps_game_log.size(), 0, title, 0.0f, 200.0f, ImVec2(300, 50));
		sprintf_s(title, 25, "Milliseconds %0.1f", ms_game_log[ms_game_log.size() - 1]);
		ImGui::PlotLines("##framerate", &ms_game_log[0], ms_game_log.size(), 0, title, 0.0f, 40.0f, ImVec2(300, 50));
		
		ImGui::SliderFloat("Time Scale", &App->time->time_scale, 0.1f, 5.0f, "%0.1f");

		ImGui::Separator();
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Time since App start: %f seconds", App->time->real_time);
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Frames since App start: %u", App->time->real_total_frame_count);

		if(App->time->game_running == Game_State::Stoped)
		{
			ImGui::TextDisabled("Time since Game start: 0.0000000 seconds");
			ImGui::TextDisabled("Frames since Game start: 0");
		}
		else
		{
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Time since Game start: %f seconds", App->time->time);
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Frames since Game start: %u", App->time->total_frame_count);
		}

	}

	if (ImGui::CollapsingHeader("Input"))
	{
		ImGui::Text("Mouse Position:");
		ImGui::Text("X: %d | Y: %d", (int)(App->input->GetMousePosition().x * App->window->screen_width), (int)(App->input->GetMousePosition().y * App->window->screen_height));
	}
	ImGui::End();
}