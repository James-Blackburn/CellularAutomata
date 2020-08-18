#version 330 core

out vec4 FragColour;

void main()
{
	float random = dot(sin(gl_FragCoord.xy), vec2(45.324f, 78.324f));
	random = fract(sin(random) * 12425.325312f);
	if (random > 0.93f)
		FragColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	else 
		FragColour = vec4(0.0f, 0.0f, 0.0f, 0.0f);
}