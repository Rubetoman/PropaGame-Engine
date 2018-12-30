#version 330 core

struct Material
{
	sampler2D diffuse_map;
	vec4 diffuse_color;
	float k_diffuse;

	sampler2D specular_map;
	vec3 specular_color;
	float shininess;
	float k_specular;

	sampler2D occlusion_map;
	float k_ambient;

    sampler2D emissive_map;
    vec3      emissive_color;
};

uniform Material material;

in float specular;

in vec2 uv0;

out vec4 color;

vec4 get_diffuse_color()
{
	return texture(material.diffuse_map, uv0)*material.diffuse_color;
}

vec3 get_specular_color()
{
	return texture(material.specular_map, uv0).rgb*material.specular_color;
}

vec3 get_occlusion_color()
{
	return texture(material.occlusion_map, uv0).rgb;
}

vec3 get_emissive_color()
{
	return texture(material.emissive_map, uv0).rgb*material.emissive_color;
}

void main()
{
    color = vec4(color.rgb*intensity, color.a);

	vec3 emissive_color = get_emissive_color();
	vec3 occlusion_color= get_occlusion_color();
	vec3 specular_color = get_specular_color();
	vec4 diffuse_color = get_diffuse_color();
    
	vec3 color_aux = emissive_color + //emissive
				 diffuse_color.rgb * occlusion_color * material.k_ambient + //ambient
				 diffuse_color.rgb * diffuse * material.k_diffuse +			//diffuse
				 specular_color.rgb * specular * material.k_specular;		//specular

    color = vec4(color_aux, diffuse_color.a);
}
