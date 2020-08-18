#version 330 core

uniform sampler2D state;
uniform vec2 scale;

out vec4 cellState;

int get(int x, int y)
{
	return int(texture(state, (gl_FragCoord.xy + vec2(x, y)) / scale).w);
}

void main()
{
	vec4 currentState = texture(state, (gl_FragCoord.xy / scale));
	
	int neighbours = get(-1, -1)+ 
					get(-1, 0) 	+
					get(-1, 1) +
					get(0, -1) +
					get(0, 0) +
					get(0, 1) +
					get(1, -1) +
					get(1, 0) +
					get(1, 1);
	
	if (currentState.w == 0.0f && (neighbours == 3 || neighbours >= 6))
		cellState = vec4(0.0f, 1.0f, 1.0f, 1.0f);
	else if (currentState.w == 0.0f && (neighbours != 3 || neighbours < 6))
		cellState = vec4(0.0f, 0.0f, 0.0f, 0.0f);
		
	else if (currentState.w == 1.0f && (neighbours >= 3 && neighbours != 5))
		cellState = vec4(0.0f, 1.0f, 1.0f, 1.0f);
	else
		cellState = vec4(0.0f, 0.0f, 0.0f, 0.0f);
}