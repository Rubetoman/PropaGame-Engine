#include "ComponentMaterial.h"

#include "Globals.h"
#include "GL/glew.h"
#include "ModuleTextures.h"

ComponentMaterial::ComponentMaterial(GameObject* go) : Component(go, component_type::Material)
{
}

ComponentMaterial::ComponentMaterial(const ComponentMaterial& comp) : Component(comp)
{
	// TODO: Create new texture or not delete it when they are used by other comp
	texture = comp.texture;
	++App->textures->textures[texture];
}

ComponentMaterial::~ComponentMaterial()
{
}

Component* ComponentMaterial::Duplicate()
{
	return new ComponentMaterial(*this);
}

bool ComponentMaterial::DrawOnInspector()
{
	ImGui::PushID(this);
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Material Component"))
	{
		bool deleted = Component::DrawOnInspector();

		if (texture != nullptr && !deleted)
		{
			ImGui::Text("Texture name: %s", texture->name);
			ImGui::Text("Texture Size:\n Width: %d | Height: %d", texture->width, texture->height);
			float panelWidth = ImGui::GetWindowContentRegionWidth();
			float conversionFactor = panelWidth / texture->width;
			ImVec2 imageSize = { texture->height *conversionFactor, panelWidth };
			ImGui::Image((ImTextureID)texture->id, imageSize);
		}
		else
		{
			ImGui::Text("No texture");
			ImGui::PopID();
			return deleted;
		}
	}
	ImGui::PopID();
	return false;
}

void ComponentMaterial::Delete()
{
	App->textures->unloadTexture(texture);
	my_go->material = nullptr;
	Component::Delete();
}