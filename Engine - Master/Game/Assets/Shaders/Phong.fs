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

in vec3 normal;
in vec3 position;
in vec2 uv0;

out vec4 color;

uniform mat4 view;

uniform	vec3 light_pos;
uniform float ambient;
uniform	Material material;

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

float lambert(vec3 direction, vec3 normals)
{
	return max(dot(normals, direction), 0.0);
}

float specular_phong(vec3 direction, vec3 pos, vec3 normals, vec3 view_pos, float shininess)
{
	vec3 view_dir = normalize(view_pos-pos);
	vec3 reflect_dir = normalize(reflect(-direction, normals));
	return pow(max(dot(view_dir, reflect_dir), 0.0), shininess);
}

void main()
{
    vec3 normal      = normalize(normal);
    vec3 light_dir   = normalize(light_pos-position);
	float distance = length(light_dir);
	light_dir = light_dir/distance;

	float diffuse = lambert(light_dir, normal);
	float specular = 0.0;
	if(diffuse> 0.0 && material.k_specular>0.0 && material.shininess > 0.0)
	{
		vec3 view_pos = transpose(mat3(view))*(-view[3].xyz);
		specular = specular_phong(light_dir, position, normal, view_pos, material.shininess);
	}

	vec3 emissive_color = get_emissive_color();
	vec3 occlusion_color= get_occlusion_color();
	vec3 specular_color = get_specular_color();
	vec4 diffuse_color = get_diffuse_color();
    
	vec3 color_aux = emissive_color + //emissive
				 diffuse_color.rgb * occlusion_color * ambient * material.k_ambient + //ambient
				 diffuse_color.rgb * diffuse * material.k_diffuse +			//diffuse
				 specular_color.rgb * specular * material.k_specular;		//specular

    color = vec4(color_aux, diffuse_color.a);
}
