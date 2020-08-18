#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

class ResourceManager
{
private:
	ResourceManager();

public:
	static int WINDOW_X;
	static int WINDOW_Y;

	static Shader ConwaysGameOfLifeShader;
	static Shader BriansBrainShader;
	static Shader DayAndNightShader;
	static Shader copyShader;
	static Shader placementShader;
	static Shader noiseShader;

	static GLFWwindow* mainWindow;

	static void setupShaders();
};

