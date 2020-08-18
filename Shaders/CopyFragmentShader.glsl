#version 330 core

uniform sampler2D state;
uniform vec2 scale;

out vec4 FragColour;

void main()
{
	vec2 dampener = abs(gl_FragCoord.xy - (scale / 2.0f));
	vec4 colour = texture(state, gl_FragCoord.xy / scale);
	if (colour.w == 0.0f) colour.rgb = vec3(0.25f, 0.25f, 0.25f);
	colour.rgb = clamp(colour.rgb - length(dampener / (scale * 2.0f)), 0.0f, 1.0f);
	FragColour = colour;
}