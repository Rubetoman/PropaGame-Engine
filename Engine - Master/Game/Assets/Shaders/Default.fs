#version 330 core

out vec4 color;

uniform vec4 newColor;
uniform sampler2D texture0;
uniform int use_color;

in vec2 uv0;

void main()
{

 if(use_color == 1)
    {
		color = newColor;
	}
	else
	{
		color = texture2D(texture0, uv0);
	}
}
