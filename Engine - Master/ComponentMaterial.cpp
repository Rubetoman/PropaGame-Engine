#include "ComponentMaterial.h"

#include "Globals.h"
#include "GL/glew.h"
#include "ModuleTextures.h"
#include "ModuleShader.h"

ComponentMaterial::ComponentMaterial(GameObject* go) : Component(go, component_type::Material)
{
}

ComponentMaterial::ComponentMaterial(const ComponentMaterial& comp) : Component(comp)
{
	program = comp.program;
	texture = comp.texture;
	color = comp.color;
	shininess = comp.shininess;
	k_specular = comp.k_specular;
	k_diffuse = comp.k_diffuse;
	k_ambient = comp.k_ambient;
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

		ImGui::ColorEdit4("object color", (float*)&color);
		ImGui::SliderFloat("shininess", &shininess, 0, 128.0f);
		ImGui::SliderFloat("K ambient", &k_ambient, 0.0f, 1.0f);
		ImGui::SliderFloat("K diffuse", &k_diffuse, 0.0f, 1.0f);
		ImGui::SliderFloat("K specular", &k_specular, 0.0f, 1.0f);

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