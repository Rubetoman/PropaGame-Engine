#ifndef __COMPOMENTMATERIAL_H__
#define __COMPOMENTMATERIAL_H__

#include "Component.h"

#include "imgui/imgui.h"

class GameObject;

struct Material 
{
	// Diffuse
	unsigned		diffuse_map = 0u;
	math::float4	diffuse_color = math::float4::one;
	float			k_diffuse = 0.5f;
	int				diffuse_width = 0;
	int				diffuse_height = 0;

	//Specular
	unsigned		specular_map = 0u;
	math::float3	specular_color = math::float3::zero;
	float			shininess = 64.0f;
	float			k_specular = 1.0f;
	int				specular_width = 0;
	int				specular_height = 0;

	// Ambient
	unsigned		occlusion_map = 0u;
	float			k_ambient = 0.5f;
	int				occlusion_width = 0;
	int				occlusion_height = 0;

	// Emissive
	unsigned		emissive_map = 0u;
	math::float3	emissive_color = math::float3::zero;
	int				emissive_width = 0;
	int				emissive_height = 0;
};

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* go);
	ComponentMaterial(const ComponentMaterial& comp);
	~ComponentMaterial();

	Component* Duplicate() override;
	void CleanUp() override;

	bool DrawOnInspector() override;
	void DrawDiffuseParameters();
	void DrawSpecularParameters();
	void DrawAmbientParameters();
	void DrawEmissiveParameters();

	void RenderMaterial();
	void Delete() override;

	JSON_value* Save(JSON_value* component) const override;
	void Load(JSON_value* component) override;

public:
	unsigned shader = 0;

	Material material;

private:
	ImVec2 image_size = { 80.0f, 80.0f };			// Size of the texture icons for ImGui
	ImVec4 info_color = { 1.0f, 1.0f, 0.0f, 1.0f };	// Color of the texture info for ImGui

	std::string diffuseSelected;
	std::string specularSelected;
	std::string occlusionSelected;
	std::string emissiveSelected;
};

#endif /*__COMPOMENTMATERIAL_H__*/