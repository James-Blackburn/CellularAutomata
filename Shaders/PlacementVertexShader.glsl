#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 viewProjection;
uniform vec2 mousePos;
uniform float circleRadius;

void main()
{
	vec3 pos = vec3((aPos.xy * circleRadius) + mousePos, aPos.z);
	gl_Position = viewProjection * vec4(pos, 1.0f);
}