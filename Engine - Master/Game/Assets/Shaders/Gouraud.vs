#version 330 

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 1) in vec2 vertex_uv0;

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

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

uniform vec3  light_pos;
uniform	Material material;

out float specular;

out vec2 uv0;
out vec3 normal;
out vec3 position;

float lambert(vec3 direction, vec3 normals)
{
	return max(dot(normals, direction), 0.0);
}

float specular_gouraud(vec3 direction, vec3 pos, vec3 normals, vec3 view_pos, float shininess)
{
        vec3 view_dir    = normalize(view_pos-pos);
		vec3 reflect_dir = reflect(-direction, normal);
		return pow(max(dot(view_dir, reflect_dir), 0.0), shininess);
}

void main()
{
    vec3 position    = (model*vec4(vertex_position, 1.0)).xyz;
    vec3 normal      = (model*vec4(vertex_normal, 0.0)).xyz;
	gl_Position      = proj*view*vec4(position, 1.0);
	uv0 = vertex_uv0;

    vec3 light_dir = normalize(light_pos-position);
	float diffuse = lambert(light_dir, normal);
	float specular = 0.0;

	if(diffuse> 0.0 && material.k_specular>0.0 && material.shininess > 0.0)
	{
		vec3 view_pos = transpose(mat3(view))*(-view[3].xyz);
		specular = specular_gouraud(light_dir, position, normal, view_pos, material.shininess);
	}
}