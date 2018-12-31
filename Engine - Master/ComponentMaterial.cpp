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
	diffuse_map = comp.diffuse_map;
	diffuse_color = comp.diffuse_color;
	shininess = comp.shininess;
	k_specular = comp.k_specular;
	k_diffuse = comp.k_diffuse;
	k_ambient = comp.k_ambient;
	++App->resources->textures[diffuse_map];
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

		
		ImGui::Separator();
		if (ImGui::TreeNode("Diffuse"))
		{
			DrawDiffuseParameters();
			ImGui::TreePop();
		}
		ImGui::Separator();

		if (ImGui::TreeNode("Specular"))
		{
			DrawSpecularParameters();
			ImGui::TreePop();
		}
		ImGui::Separator();

		if (ImGui::TreeNode("Ambient"))
		{
			DrawAmbientParameters();
			ImGui::TreePop();
		}
		ImGui::Separator();

		if (ImGui::TreeNode("Emissive"))
		{
			DrawEmissiveParameters();
			ImGui::TreePop();
		}
		ImGui::Separator();
	}
	ImGui::PopID();
	return false;
}

void ComponentMaterial::DrawDiffuseParameters()
{
	// Texture
	if (diffuse_map != nullptr)
	{
		ImGui::Columns(2, "diffuse_column", false);  // 2-ways, no border
		// Texture image button
		if (ImGui::ImageButton((ImTextureID)diffuse_map->id, image_size, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), -1, ImColor(0, 0, 0, 255)))
			ImGui::OpenPopup("Textures");

		// Show texture info
		ImGui::NextColumn();
		ImGui::Text("Texture:");
		ImGui::TextColored(info_color, "%s", diffuse_map->name);
		ImGui::TextColored(info_color, "(%d x %d) %s", diffuse_map->width, diffuse_map->height, diffuse_map->format);
		ImGui::Checkbox("Mipmaps", &diffuse_map->use_mipmap);
		// Button to remove texture
		if (ImGui::Button("Delete"))
		{
			App->textures->unloadTexture(diffuse_map);
			diffuse_map = nullptr;
		}
		ImGui::Columns(1);

	}
	else
	{
		ImGui::Columns(2, "diffuse_column", false);  // 2-ways, no border
		if (ImGui::ImageButton((ImTextureID)-1, image_size, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), -1, ImColor(0, 0, 0, 255)))
			ImGui::OpenPopup("Textures");
		ImGui::NextColumn();
		ImGui::Text("Texture:");
		ImGui::Columns(1);
	}
	ImGui::ColorEdit4("diffuse color", (float*)&diffuse_color);
	ImGui::SliderFloat("K diffuse", &k_diffuse, 0.0f, 1.0f);

	// Textures popup
	if (ImGui::BeginPopup("Textures", NULL))
	{
		for (auto texture = App->resources->textures.begin(); texture != App->resources->textures.end(); texture++)
		{
			if (ImGui::ImageButton((ImTextureID)texture->first->id, image_size, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), -1, ImColor(0, 0, 0, 255)))
			{
				if (texture->first != diffuse_map)
				{
					App->textures->unloadTexture(diffuse_map);
					diffuse_map = texture->first;
					if (texture->second > 0)
						++texture->second;
				}
			}
		}
		ImGui::EndPopup();
	}
}

void ComponentMaterial::DrawSpecularParameters()
{
	// Texture
	if (specular_map != nullptr)
	{
		ImGui::Columns(2, "specular_column", false);  // 2-ways, no border

		if (ImGui::ImageButton((ImTextureID)specular_map->id, image_size, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), -1, ImColor(0, 0, 0, 255)))
			ImGui::OpenPopup("Textures");
		// Show texture info
		ImGui::NextColumn();
		ImGui::Text("Texture:");
		ImGui::TextColored(info_color, "%s", specular_map->name);
		ImGui::TextColored(info_color, "(%d x %d) %s", specular_map->width, specular_map->height, specular_map->format);
		ImGui::Checkbox("Mipmaps", &specular_map->use_mipmap);
		// Button to remove texture
		if (ImGui::Button("Delete"))
		{
			App->textures->unloadTexture(specular_map);
			specular_map = nullptr;
		}
		ImGui::Columns(1);
	}
	else
	{
		ImGui::Columns(2, "specular_column", false);  // 2-ways, no border
		if (ImGui::ImageButton((ImTextureID)-1, image_size, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), -1, ImColor(0, 0, 0, 255)))
			ImGui::OpenPopup("Textures");
		ImGui::NextColumn();
		ImGui::Text("Texture:");
		ImGui::Columns(1);
	}

	ImGui::ColorEdit3("specular color", (float*)&specular_color);
	ImGui::SliderFloat("K specular", &k_specular, 0.0f, 1.0f);
	ImGui::SliderFloat("shininess", &shininess, 0, 128.0f);

	// Textures popup
	if (ImGui::BeginPopup("Textures", NULL))
	{
		for (auto texture = App->resources->textures.begin(); texture != App->resources->textures.end(); texture++)
		{
			if (ImGui::ImageButton((ImTextureID)texture->first->id, image_size, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), -1, ImColor(0, 0, 0, 255)))
			{
				if (texture->first != specular_map)
				{
					App->textures->unloadTexture(specular_map);
					specular_map = texture->first;
					if (texture->second > 0)
						++texture->second;
				}
			}
		}
		ImGui::EndPopup();
	}
}

void ComponentMaterial::DrawAmbientParameters()
{
		// Texture
		if (occlusion_map != nullptr)
		{
			ImGui::Columns(2, "occlusion_column", false);  // 2-ways, no border
			if (ImGui::ImageButton((ImTextureID)occlusion_map->id, image_size, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), -1, ImColor(0, 0, 0, 255)))
				ImGui::OpenPopup("Textures");
			// Show texture info
			ImGui::NextColumn();
			ImGui::Text("Texture:");
			ImGui::TextColored(info_color, "%s", occlusion_map->name);
			ImGui::TextColored(info_color, "(%d x %d) %s", occlusion_map->width, occlusion_map->height, occlusion_map->format);
			ImGui::Checkbox("Mipmaps", &occlusion_map->use_mipmap);
			// Button to remove texture
			if (ImGui::Button("Delete"))
			{
				App->textures->unloadTexture(occlusion_map);
				occlusion_map = nullptr;
			}
			ImGui::Columns(1);
		}
		else
		{
			ImGui::Columns(2, "occlusion_column", false);  // 2-ways, no border
			if (ImGui::ImageButton((ImTextureID)-1, image_size, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), -1, ImColor(0, 0, 0, 255)))
				ImGui::OpenPopup("Textures");
			ImGui::NextColumn();
			ImGui::Text("Texture:");
			ImGui::Columns(1);
		}

		ImGui::SliderFloat("K ambient", &k_ambient, 0.0f, 1.0f);

		// Textures popup
		if (ImGui::BeginPopup("Textures", NULL))
		{
			for (auto texture = App->resources->textures.begin(); texture != App->resources->textures.end(); texture++)
			{
				if (ImGui::ImageButton((ImTextureID)texture->first->id, image_size, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), -1, ImColor(0, 0, 0, 255)))
				{
					if (texture->first != occlusion_map)
					{
						App->textures->unloadTexture(occlusion_map);
						occlusion_map = texture->first;
						if (texture->second > 0)
							++texture->second;
					}
				}
			}
			ImGui::EndPopup();
		}
}

void ComponentMaterial::DrawEmissiveParameters()
{
	// Texture
	if (emissive_map != nullptr)
	{
		ImGui::Columns(2, "emissive_column", false);  // 2-ways, no border
		if (ImGui::ImageButton((ImTextureID)emissive_map->id, image_size, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), -1, ImColor(0, 0, 0, 255)))
			ImGui::OpenPopup("Textures");
		// Show texture info
		ImGui::NextColumn();
		ImGui::Text("Texture:");
		ImGui::TextColored(info_color, "%s", emissive_map->name);
		ImGui::TextColored(info_color, "(%d x %d) %s", emissive_map->width, emissive_map->height, emissive_map->format);
		ImGui::Checkbox("Mipmaps", &emissive_map->use_mipmap);
		// Button to remove texture
		if (ImGui::Button("Delete"))
		{
			App->textures->unloadTexture(emissive_map);
			emissive_map = nullptr;
		}
		ImGui::Columns(1);
	}
	else
	{
		ImGui::Columns(2, "emissive_column", false);  // 2-ways, no border
		if (ImGui::ImageButton((ImTextureID)-1, image_size, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), -1, ImColor(0, 0, 0, 255)))
			ImGui::OpenPopup("Textures");
		ImGui::NextColumn();
		ImGui::Text("Texture:");
		ImGui::Columns(1);
	}
	ImGui::ColorEdit3("emissive color", (float*)&emissive_color);

	// Textures popup
	if (ImGui::BeginPopup("Textures", NULL))
	{
		for (auto texture = App->resources->textures.begin(); texture != App->resources->textures.end(); texture++)
		{
			if (ImGui::ImageButton((ImTextureID)texture->first->id, image_size, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), -1, ImColor(0, 0, 0, 255)))
			{
				if (texture->first != emissive_map)
				{
					App->textures->unloadTexture(emissive_map);
					emissive_map = texture->first;
					if (texture->second > 0)
						++texture->second;
				}
			}
		}
		ImGui::EndPopup();
	}
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

	// Diffuse
	glUniform4fv(glGetUniformLocation(program, "material.diffuse_color"), 1, (GLfloat*)&diffuse_color);
	if (diffuse_map != nullptr)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuse_map->id);
		glUniform1i(glGetUniformLocation(program, "material.diffuse_map"), 0);
	}

	// Specular
	glUniform3fv(glGetUniformLocation(program, "material.specular_color"), 1, (GLfloat*)&specular_color);
	if (specular_map != nullptr)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specular_map->id);
		glUniform1i(glGetUniformLocation(program, "material.specular_map"), 1);
	}

	// Ambient
	if (occlusion_map != nullptr)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, occlusion_map->id);
		glUniform1i(glGetUniformLocation(program, "material.occlusion_map"), 2);
	}

	// Emissive
	glUniform3fv(glGetUniformLocation(program, "material.emissive_color"), 1, (GLfloat*)&specular_color);
	if (emissive_map != nullptr)
	{
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, emissive_map->id);
		glUniform1i(glGetUniformLocation(program, "material.emissive_map"), 3);
	}

	glUniform1f(glGetUniformLocation(program, "material.shininess"), shininess);
	glUniform1f(glGetUniformLocation(program, "material.k_ambient"), k_ambient);
	glUniform1f(glGetUniformLocation(program, "material.k_diffuse"), k_diffuse);
	glUniform1f(glGetUniformLocation(program, "material.k_specular"), k_specular);
}

void ComponentMaterial::Delete()
{
	App->textures->unloadTexture(diffuse_map);
	my_go->material = nullptr;
	Component::Delete();
}

JSON_value* ComponentMaterial::Save(JSON_value* component) const
{
	JSON_value* material = Component::Save(component);

	material->AddUnsigned("shader", shader);

	if(diffuse_map != nullptr)
		material->AddString("texture", (diffuse_map->path));

	material->AddVec4("color", diffuse_color);
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
	
	// Get texture
	const char* tx = component->GetString("texture");
	if(tx != nullptr)
		diffuse_map = App->textures->loadTexture(component->GetString("texture"), false);

	diffuse_color = component->GetVec4("color");
	shininess = component->GetFloat("shininess");
	k_specular = component->GetFloat("k_specular");
	k_diffuse = component->GetFloat("k_diffuse");
	k_ambient = component->GetFloat("k_ambient");
}