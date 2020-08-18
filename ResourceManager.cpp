#include "ResourceManager.h"

int ResourceManager::WINDOW_X = 0;
int ResourceManager::WINDOW_Y = 0;

Shader ResourceManager::ConwaysGameOfLifeShader;
Shader ResourceManager::BriansBrainShader;
Shader ResourceManager::DayAndNightShader;
Shader ResourceManager::copyShader;
Shader ResourceManager::placementShader;
Shader ResourceManager::noiseShader;

GLFWwindow* ResourceManager::mainWindow = nullptr;

void ResourceManager::setupShaders()
{
	ConwaysGameOfLifeShader.addShader(GL_VERTEX_SHADER, "Shaders/ConwaysGOLVertexShader.glsl");
	ConwaysGameOfLifeShader.addShader(GL_FRAGMENT_SHADER, "Shaders/ConwaysGOLFragmentShader.glsl");
	ConwaysGameOfLifeShader.linkProgram();

	BriansBrainShader.addShader(GL_VERTEX_SHADER, "Shaders/BriansBrainVertexShader.glsl");
	BriansBrainShader.addShader(GL_FRAGMENT_SHADER, "Shaders/BriansBrainFragmentShader.glsl");
	BriansBrainShader.linkProgram();

	DayAndNightShader.addShader(GL_VERTEX_SHADER, "Shaders/DayAndNightVertexShader.glsl");
	DayAndNightShader.addShader(GL_FRAGMENT_SHADER, "Shaders/DayAndNightFragmentShader.glsl");
	DayAndNightShader.linkProgram();

	copyShader.addShader(GL_VERTEX_SHADER, "Shaders/CopyVertexShader.glsl");
	copyShader.addShader(GL_FRAGMENT_SHADER, "Shaders/CopyFragmentShader.glsl");
	copyShader.linkProgram();

	placementShader.addShader(GL_VERTEX_SHADER, "Shaders/PlacementVertexShader.glsl");
	placementShader.addShader(GL_FRAGMENT_SHADER, "Shaders/PlacementFragmentShader.glsl");
	placementShader.linkProgram();

	noiseShader.addShader(GL_VERTEX_SHADER, "Shaders/NoiseVertexShader.glsl");
	noiseShader.addShader(GL_FRAGMENT_SHADER, "Shaders/NoiseFragmentShader.glsl");
	noiseShader.linkProgram();
}
