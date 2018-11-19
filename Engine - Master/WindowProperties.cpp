#include "WindowProperties.h"
#include "ModuleModelLoader.h"
#include "ModuleTextures.h"
#include "Texture.h"

WindowProperties::WindowProperties(const char* name) : Window(name)
{
	active = true;
}

WindowProperties::~WindowProperties()
{
}

void WindowProperties::Draw()
{
	ImGui::Begin("Properties", &active, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_AlwaysVerticalScrollbar);

	if (App->model_loader->meshes.size() > 0)
	{
		ImGui::Text("Current loaded file has %d meshes", App->model_loader->meshes.size());
		ImGui::NewLine();
		int count = 0;
		for (std::vector<ModuleModelLoader::mesh*>::iterator it_m = App->model_loader->meshes.begin(); it_m != App->model_loader->meshes.end(); ++it_m)
		{
			ModuleModelLoader::mesh* mesh = (*it_m);
			ImGui::Text("Mesh name: %s", mesh->name.c_str());

			ImGui::PushID("Transformation" + count);
			if (ImGui::CollapsingHeader("Transformation"))
			{
				ImGui::Text("Position:\n X: %f | Y: %f | Z: %f", mesh->position.x, mesh->position.y, mesh->position.z);
				//float3 rot = mesh->rotation.ToEulerXYZ();
				//rot *= 180 / pi;
				//ImGui::Text("Rotation:\n X: %f | Y: %f | Z: %f", rot.x, rot.y, rot.z);
				ImGui::Text("Scale:\n X: %f | Y: %f | Z: %f", mesh->scale.x, mesh->scale.y, mesh->scale.z);
			}
			ImGui::PopID();
			ImGui::PushID("Geometry" + count);
			if (ImGui::CollapsingHeader("Geometry"))
			{
				ImGui::Text("Triangles Count: %d", mesh->num_indices / 3);
				ImGui::Text("Vertices Count: %d", mesh->num_vertices);
				ImGui::Text("Mesh size:\n X: %f | Y: %f | Z: %f", mesh->boundingBox.Size().x, mesh->boundingBox.Size().y, mesh->boundingBox.Size().z);
			}
			ImGui::PopID();
			ImGui::PushID("Texture" + count);
			if (ImGui::CollapsingHeader("Texture"))
			{
				if (mesh->texture != 0)
				{
					ImGui::Text("Texture name: %s", mesh->texture->name);
					ImGui::Text("Texture Size:\n Width: %d | Height: %d", mesh->texture->width, mesh->texture->height);
					float panelWidth = ImGui::GetWindowContentRegionWidth();
					float conversionFactor = panelWidth / mesh->texture->width;
					ImVec2 imageSize = { mesh->texture->height *conversionFactor, panelWidth };
					ImGui::Image((ImTextureID)mesh->texture->id, imageSize);
				}
				else
					ImGui::Text("No texture");
			}
			ImGui::PopID();

			count++;
			ImGui::NewLine();
		}
	}
	else
		ImGui::Text("No meshes loaded");

	ImGui::End();
}