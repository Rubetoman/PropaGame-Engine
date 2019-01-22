#include "WindowCamera.h"

#include "Application.h"
#include "ModuleInput.h"
#include "ModuleCamera.h"
#include "ModuleResources.h"
#include "ModuleWindow.h"

#include "ComponentCamera.h"

#include "GameObject.h"
#include "SDL.h"

WindowCamera::WindowCamera(const char* name) : Window(name)
{
}

WindowCamera::~WindowCamera()
{
	camera = nullptr;
}

void WindowCamera::Draw()
{
	ImGui::Begin(camera->my_go->name.c_str(), &active, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	ImVec2 size = ImGui::GetWindowSize();
	ImGui::SetCursorPos({ -(App->window->screen_width - size.x) / 2,-(App->window->screen_height - size.y) / 2 });
	
	unsigned renderedTexture = App->resources->no_camera_texture;

	if (camera != nullptr)
	{
		if (camera->my_go->isActive() && camera->active)
			renderedTexture = camera->renderedTexture;
	}

	ImGui::Image((ImTextureID)renderedTexture, { (float)App->window->screen_width, (float)App->window->screen_height }, { 0,1 }, { 1,0 });

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) != KEY_REPEAT && App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) != KEY_REPEAT && App->input->GetMouseButtonDown(SDL_BUTTON_MIDDLE) != KEY_REPEAT)
		focus = ImGui::IsMouseHoveringWindow();

	ImGui::End();
}

bool WindowCamera::IsFocused() const { return focus; }