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

	App->scene->DrawImGuizmo(ImGuizmo::OPERATION::TRANSLATE);

	ImGui::End();
}

bool WindowScene::IsFocused() const { return focus; }