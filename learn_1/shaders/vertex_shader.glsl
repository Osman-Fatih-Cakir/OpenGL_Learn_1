#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 vcolor;

uniform mat4 projection;
uniform mat4 modelview;

uniform vec3 translation;
uniform vec3 rotation;

out vec4 fcolor;

void main()
{
	gl_Position = projection * modelview * vec4(position, 1.0f);
	fcolor = vec4(vcolor, 1.0f);
}