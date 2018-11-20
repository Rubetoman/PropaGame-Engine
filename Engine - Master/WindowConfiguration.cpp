#include "WindowConfiguration.h"
#include "ModuleWindow.h"

WindowConfiguration::WindowConfiguration(const char* name) : Window(name)
{
	active = true;
}

WindowConfiguration::~WindowConfiguration()
{
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
	if (ImGui::CollapsingHeader("Camera"))
	{
		ImGui::Text("Camera Position:");
		ImGui::Text("X: "); ImGui::SameLine();
		ImGui::Text(std::to_string(App->camera->mainCamera->position.x).c_str());
		ImGui::SameLine(100); ImGui::Text("Y: "); ImGui::SameLine();
		ImGui::Text(std::to_string(App->camera->mainCamera->position.y).c_str());
		ImGui::SameLine(200); ImGui::Text("Z: "); ImGui::SameLine();
		ImGui::Text(std::to_string(App->camera->mainCamera->position.z).c_str());
		ImGui::NewLine();
		ImGui::Separator();

		// Front, side and up vectors
		ImGui::Text("Camera Vectors:");
		ImGui::Text("Front: ");
		ImGui::Text("X: "); ImGui::SameLine();
		ImGui::Text(std::to_string(App->camera->mainCamera->front.x).c_str());
		ImGui::SameLine(100); ImGui::Text("Y: "); ImGui::SameLine();
		ImGui::Text(std::to_string(App->camera->mainCamera->front.y).c_str());
		ImGui::SameLine(200); ImGui::Text("Z: "); ImGui::SameLine();
		ImGui::Text(std::to_string(App->camera->mainCamera->front.z).c_str());
		ImGui::NewLine();

		ImGui::Text("Side: ");
		ImGui::Text("X: "); ImGui::SameLine();
		ImGui::Text(std::to_string(App->camera->mainCamera->side.x).c_str());
		ImGui::SameLine(100); ImGui::Text("Y: "); ImGui::SameLine();
		ImGui::Text(std::to_string(App->camera->mainCamera->side.y).c_str());
		ImGui::SameLine(200); ImGui::Text("Z: "); ImGui::SameLine();
		ImGui::Text(std::to_string(App->camera->mainCamera->side.z).c_str());
		ImGui::NewLine();

		ImGui::Text("Up: ");
		ImGui::Text("X: "); ImGui::SameLine();
		ImGui::Text(std::to_string(App->camera->mainCamera->up.x).c_str());
		ImGui::SameLine(100); ImGui::Text("Y: "); ImGui::SameLine();
		ImGui::Text(std::to_string(App->camera->mainCamera->up.y).c_str());
		ImGui::SameLine(200); ImGui::Text("Z: "); ImGui::SameLine();
		ImGui::Text(std::to_string(App->camera->mainCamera->up.z).c_str());
		ImGui::NewLine();
		ImGui::Separator();

		ImGui::Text("Camera Vectors:");
		ImGui::Text("Pitch: "); ImGui::SameLine();
		ImGui::Text(std::to_string(App->camera->mainCamera->pitch).c_str());
		ImGui::Text("Yaw: "); ImGui::SameLine();
		ImGui::Text(std::to_string(App->camera->mainCamera->yaw).c_str());
		ImGui::NewLine();
		ImGui::Separator();

		ImGui::PushItemWidth(100.0f);
		ImGui::InputFloat("Camera Speed", &App->camera->mainCamera->speed);
		ImGui::InputFloat("Mouse Sensitivity", &App->camera->mouse_sensitivity);
		ImGui::PopItemWidth();
	}

	if (ImGui::CollapsingHeader("Mesh"))
	{
		if (ImGui::Button("Use Chekers Texture"))
		{
			App->model_loader->ChangeMeshTexture("Checkers_Texture.jpg");
		}
	}

	if (ImGui::CollapsingHeader("Input"))
	{
		ImGui::Text("Mouse Position:");
		ImGui::Text("X: %d | Y: %d", App->input->GetMousePosition().x, App->input->GetMousePosition().y);
	}

	if (ImGui::CollapsingHeader("Textures"))
	{
		ImGui::Text("Loaded textures:");
		for (std::vector<Texture*>::iterator it_t = App->textures->textures.begin(); it_t != App->textures->textures.end(); ++it_t)
		{
			Texture* texture = (*it_t);
			if (ImGui::CollapsingHeader(texture->name))
			{
				ImGui::Text("Texture name: %s", texture->name);
				ImGui::Text("Texture path: %s", texture->path);
				ImGui::Text("Texture Size:\n Width: %d | Height: %d", texture->width, texture->height);
				float panelWidth = ImGui::GetWindowContentRegionWidth();
				float conversionFactor = panelWidth / texture->width;
				ImVec2 imageSize = { texture->height *conversionFactor, panelWidth };
				ImGui::Image((ImTextureID)texture->id, imageSize);
			}
			ImGui::NewLine();
		}
	}
	ImGui::End();
}