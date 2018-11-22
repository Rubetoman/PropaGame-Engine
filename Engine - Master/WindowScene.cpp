#include "WindowScene.h"

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

	ImGui::Image((ImTextureID)App->renderer->renderedTexture, { (float)App->window->screen_width, (float)App->window->screen_height }, { 0,1 }, { 1,0 });


	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) != KEY_REPEAT && App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) != KEY_REPEAT && App->input->GetMouseButtonDown(SDL_BUTTON_MIDDLE) != KEY_REPEAT)
		focus = ImGui::IsMouseHoveringWindow();

	ImGui::End();
}

bool WindowScene::IsFocused() const { return focus; }