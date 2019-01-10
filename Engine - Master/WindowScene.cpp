#include "WindowScene.h"

#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleResources.h"
#include "ModuleScene.h"

#include "WindowHierarchy.h"

#include "ComponentCamera.h"

#include "ImGuizmo/ImGuizmo.h"

WindowScene::WindowScene(const char* name) : Window(name)
{
	active = true;
}

WindowScene::~WindowScene()
{}

void WindowScene::Draw()
{
	ImGui::Begin(name.c_str(), &active, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	ImVec2 size = ImGui::GetWindowSize();
	ImGui::SetCursorPos({ -(App->window->screen_width - size.x) / 2,-(App->window->screen_height - size.y) / 2 });
	viewport.x = ImGui::GetCursorPosX() + ImGui::GetWindowPos().x;
	viewport.y = ImGui::GetCursorPosY() + ImGui::GetWindowPos().y;

	if(App->camera != nullptr && App->camera->editor_camera_comp != nullptr)
		ImGui::Image((ImTextureID)App->camera->editor_camera_comp->renderedTexture, { (float)App->window->screen_width, (float)App->window->screen_height }, { 0,1 }, { 1,0 });
	else if(App->resources != nullptr)
		ImGui::Image((ImTextureID)App->resources->no_camera_texture, { (float)App->window->screen_width, (float)App->window->screen_height }, { 0,1 }, { 1,0 });

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) != KEY_REPEAT && App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) != KEY_REPEAT && App->input->GetMouseButtonDown(SDL_BUTTON_MIDDLE) != KEY_REPEAT)
		focus = ImGui::IsMouseHoveringWindow();

	// Avoid deselecting a GO when clicking on guizmos
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT))
	{
		gui_click = ImGuizmo::IsOver();
	}

	// Guizmo mode buttons
	static ImGuizmo::OPERATION operation(ImGuizmo::TRANSLATE);

	ImGui::SetCursorPos(ImGui::GetWindowContentRegionMin());

	if(operation == ImGuizmo::TRANSLATE)
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.1f, 1.0f, 1.0f, 0.6f));
	else		
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.1f, 1.0f, 1.0f, 0.3f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.1f, 1.0f, 1.0f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.1f, 1.0f, 1.0f, 1.0f));
	if (ImGui::Button("Translate"))
	{
		operation = ImGuizmo::TRANSLATE;
	}
	if (ImGui::IsItemHovered()) gui_click = true;
	ImGui::PopStyleColor(3);
	ImGui::SameLine();

	if (operation == ImGuizmo::ROTATE)
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.1f, 1.0f, 1.0f, 0.6f));
	else
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.1f, 1.0f, 1.0f, 0.3f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.1f, 1.0f, 1.0f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.1f, 1.0f, 1.0f, 1.0f));
	if (ImGui::Button("Rotate"))
	{
		operation = ImGuizmo::ROTATE;
	}
	if (ImGui::IsItemHovered()) gui_click = true;
	ImGui::PopStyleColor(3);
	ImGui::SameLine();

	if (operation == ImGuizmo::SCALE)
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.1f, 1.0f, 1.0f, 0.6f));
	else
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.1f, 1.0f, 1.0f, 0.3f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.1f, 1.0f, 1.0f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.1f, 1.0f, 1.0f, 1.0f));
	if (ImGui::Button("Scale"))
	{
		operation = ImGuizmo::SCALE;
	}
	if(ImGui::IsItemHovered()) gui_click = true;
	ImGui::PopStyleColor(3);

	// Draw guizmo
	App->scene->DrawImGuizmo(operation);

	ImGui::End();
}

bool WindowScene::IsFocused() const { return focus; }