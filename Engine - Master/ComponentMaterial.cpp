#include "ComponentMaterial.h"

#include "Globals.h"
#include "GL/glew.h"

ComponentMaterial::ComponentMaterial(GameObject* go) : Component(go, component_type::Material)
{
}


ComponentMaterial::~ComponentMaterial()
{
	Delete();
}

void ComponentMaterial::DrawOnInspector()
{
	ImGui::PushID(this);
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Material Component"))
	{
		Component::DrawOnInspector();

		if (texture != nullptr)
		{
			ImGui::Text("Texture name: %s", texture->name);
			ImGui::Text("Texture Size:\n Width: %d | Height: %d", texture->width, texture->height);
			float panelWidth = ImGui::GetWindowContentRegionWidth();
			float conversionFactor = panelWidth / texture->width;
			ImVec2 imageSize = { texture->height *conversionFactor, panelWidth };
			ImGui::Image((ImTextureID)texture->id, imageSize);
		}
		else
			ImGui::Text("No texture");
	}
	ImGui::PopID();
}

void ComponentMaterial::Delete()
{
	if (texture != nullptr)
	{
		glDeleteTextures(1, (GLuint*)&texture->id);
	}
	RELEASE(texture);
	my_go->material = nullptr;
	Component::Delete();
}