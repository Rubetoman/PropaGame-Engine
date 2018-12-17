#include "ComponentMaterial.h"

#include "Globals.h"
#include "GL/glew.h"

#include "ComponentTransform.h"
#include "ComponentLight.h"

#include "ModuleTextures.h"
#include "ModuleResources.h"
#include "ModuleShader.h"

ComponentMaterial::ComponentMaterial(GameObject* go) : Component(go, component_type::Material)
{
}

ComponentMaterial::ComponentMaterial(const ComponentMaterial& comp) : Component(comp)
{
	shader = comp.shader;
	texture = comp.texture;
	color = comp.color;
	shininess = comp.shininess;
	k_specular = comp.k_specular;
	k_diffuse = comp.k_diffuse;
	k_ambient = comp.k_ambient;
	++App->resources->textures[texture];
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

		// Shader
		char* program_names[ModuleShader::PROGRAM_COUNT] = { "Default", "Flat", "Gouraud", "Phong", "Blinn"};
		ImGui::Combo("shader", (int*)&shader, program_names, ModuleShader::PROGRAM_COUNT);

		ImGui::ColorEdit4("object color", (float*)&color);
		ImGui::SliderFloat("shininess", &shininess, 0, 128.0f);
		ImGui::SliderFloat("K ambient", &k_ambient, 0.0f, 1.0f);
		ImGui::SliderFloat("K diffuse", &k_diffuse, 0.0f, 1.0f);
		ImGui::SliderFloat("K specular", &k_specular, 0.0f, 1.0f);
		ImGui::Separator();

		// Texture
		if (texture != nullptr && !deleted)
		{
			// Button to remove texture
			if (ImGui::Button("Remove Texture"))
			{
				App->textures->unloadTexture(texture);
				texture = nullptr;
			}
			else
			{
				// Show texture info
				ImGui::Text("Texture name: %s", texture->name);
				ImGui::Text("Texture Size:\n Width: %d | Height: %d", texture->width, texture->height);
				float panelWidth = ImGui::GetWindowContentRegionWidth();
				float conversionFactor = panelWidth / texture->width;
				ImVec2 imageSize = { texture->height *conversionFactor, panelWidth };
				ImGui::Image((ImTextureID)texture->id, imageSize);
			}
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

void ComponentMaterial::RenderMaterial()
{
	unsigned program = App->shader->programs[shader];

	// Light render
	GameObject* light = App->resources->lights[0];
	if (light != nullptr && light->active)
	{
		glUniform3fv(glGetUniformLocation(program, "light_pos"), 1, (const float*)&light->transform->position);
		
		ComponentLight* comp_light = (ComponentLight*)light->GetComponent(component_type::Light);
		if (comp_light != nullptr && comp_light->active)
			glUniform1f(glGetUniformLocation(program, "ambient"), comp_light->intensity);
		else
			glUniform1f(glGetUniformLocation(program, "ambient"), 0.0f);
	}
	else
	{
		glUniform1f(glGetUniformLocation(program, "ambient"), 0.0f);
	}

	glUniform1f(glGetUniformLocation(program, "shininess"), shininess);
	glUniform1f(glGetUniformLocation(program, "k_ambient"), k_ambient);
	glUniform1f(glGetUniformLocation(program, "k_diffuse"), k_diffuse);
	glUniform1f(glGetUniformLocation(program, "k_specular"), k_specular);

	if (texture == nullptr)
	{
		glUniform1i(glGetUniformLocation(program, "use_diffuse_map"),0);
		glUniform4fv(glGetUniformLocation(program, "object_color"), 1, (GLfloat*)&color);
	}
	else
	{
		glUniform1i(glGetUniformLocation(program, "use_diffuse_map"), 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->id);
		glUniform1i(glGetUniformLocation(program, "diffuse_map"), 0);
	}
}

void ComponentMaterial::Delete()
{
	App->textures->unloadTexture(texture);
	my_go->material = nullptr;
	Component::Delete();
}

JSON_value* ComponentMaterial::Save(JSON_value* component) const
{
	JSON_value* material = Component::Save(component);

	material->AddUnsigned("shader", shader);
	material->AddUnsigned("texture", (unsigned)texture);
	material->AddVec4("color", color);
	material->AddFloat("shininess", shininess);
	material->AddFloat("k_specular", k_specular);
	material->AddFloat("k_diffuse", k_diffuse);
	material->AddFloat("k_ambient", k_ambient);

	component->addValue("", material);

	return material;
}

void ComponentMaterial::Load(JSON_value* component)
{
	Component::Load(component);

	shader = component->GetUnsigned("shader");
	texture = (Texture*)component->GetUnsigned("texture");
	color = component->GetVec4("color");
	shininess = component->GetFloat("shininess");
	k_specular = component->GetFloat("k_specular");
	k_diffuse = component->GetFloat("k_diffuse");
	k_ambient = component->GetFloat("k_ambient");
}