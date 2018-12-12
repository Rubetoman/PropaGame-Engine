#version 330 core

uniform sampler2D diffuse_map;
uniform vec4 object_color;
uniform int use_diffuse_map;

flat in float intensity;

in vec2 uv0;

out vec4 color;

void main()
{
	if(use_diffuse_map == 1)
    {
		color = texture2D(diffuse_map, uv0);
	}
	else
	{
		color = object_color;
	}

	color = vec4(color.rgb*intensity, color.a);
}
