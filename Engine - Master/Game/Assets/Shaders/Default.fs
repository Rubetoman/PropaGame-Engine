#version 330 core
#define MAX_POINT_LIGHTS 4
#define MAX_SPOT_LIGHTS 4

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

struct DirLight
{
	vec3 direction;
	vec3 color;
};

struct PointLight
{
	vec3  position;
	vec3  color;
	vec3  attenuation; //constant + linear + quadratic
};

struct SpotLight
{
	vec3  position;
	vec3  direction;
	vec3  color;
	vec3  attenuation;
	float inner;
	float outer;
};

struct Lights
{
	vec3        ambient_color; 
	DirLight    directional;
	PointLight  points[MAX_POINT_LIGHTS];
	int         num_points;
	SpotLight   spots[MAX_SPOT_LIGHTS];
	int         num_spots;
};

layout (std140) uniform Matrices
{
    mat4 proj;
    mat4 view;
};

in vec3 normal;
in vec3 position;
in vec2 uv0;

out vec4 color;

uniform	Material material;
uniform Lights lights;

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

float get_attenuation(vec3 attenuation, float distance)
{
	return 1/(attenuation[0] + distance * attenuation[1] + distance*distance*attenuation[2]);
}

float lambert(vec3 direction, vec3 normals)
{
	return max(dot(normals, direction), 0.0);
}

float specular_phong(vec3 direction, vec3 pos, vec3 normals, vec3 view_pos, float shininess)
{
	vec3 view_dir = normalize(view_pos-pos);
	vec3 half_dir = normalize(view_dir+direction);
	return pow(max(dot(normals, half_dir), 0.0), shininess);
}

vec3 directional_phong(vec3 normal, vec3 viewPos, DirLight dir, vec4 diffuse_color, vec3 specular_color)
{
	float diffuse = lambert(dir.direction, normal);
	float specular = specular_phong(dir.direction, position, normal, viewPos, material.shininess);

	vec3 color = dir.color * (diffuse_color.rgb * diffuse * material.k_diffuse + //diffuse
				 specular_color.rgb * specular * material.k_specular); //specular
	
	return color;
}

vec3 point_phong(vec3 normal, vec3 viewPos, PointLight point, vec4 diffuse_color, vec3 specular_color)
{
	vec3 lightDir = point.position - position;
	float distance = length(lightDir);
	lightDir = lightDir/distance;
	float att = get_attenuation(point.attenuation, distance);

	float diffuse = lambert(lightDir, normal);
	float specular = specular_phong(lightDir, position, normal, viewPos, material.shininess);

	vec3 color = att * point.color * (diffuse_color.rgb *(diffuse * material.k_diffuse +
				 specular_color.rgb * specular * material.k_specular));
	return color;
}

vec3 spot_phong(vec3 normal, vec3 viewPos, SpotLight spot, vec4 diffuse_color, vec3 specular_color)
{
	vec3 lightDir = spot.position - position;
	float distance = length(lightDir);
	lightDir = lightDir/distance;

	float theta = dot(normalize(lightDir), normalize(-spot.direction));
	float epsilon = max(0.0001, spot.inner-spot.outer);
	float cone = clamp((theta - spot.outer) / epsilon, 0.0, 1.0); 
	
	float att = get_attenuation(spot.attenuation, distance)* cone;

	float diffuse = lambert(lightDir, normal);
	float specular = specular_phong(lightDir, position, normal, viewPos, material.shininess);

	vec3 color = att * spot.color * (diffuse_color.rgb *(diffuse * material.k_diffuse +
				specular_color.rgb * specular * material.k_specular));

	return color;
}

void main()
{
    vec3 normal      = normalize(normal);
	vec3 view_pos = transpose(mat3(view))*(-view[3].xyz);

	vec3 emissive_color = get_emissive_color();
	vec3 occlusion_color= get_occlusion_color();
	vec3 specular_color = get_specular_color();
	vec4 diffuse_color = get_diffuse_color();

	vec3 aux_color = directional_phong(normal, view_pos, lights.directional, diffuse_color, specular_color);

		for(int i=0; i < lights.num_points; ++i)
	{
		aux_color+= point_phong(normal, view_pos, lights.points[i], diffuse_color, specular_color);
	}

	for(int i=0; i < lights.num_spots; ++i)
	{
		aux_color+= spot_phong(normal, view_pos, lights.spots[i], diffuse_color, specular_color);
	}
    
	aux_color += emissive_color + //emissive
				 diffuse_color.rgb * lights.ambient_color * occlusion_color * material.k_ambient; //ambient

    color = vec4(aux_color, diffuse_color.a);
}