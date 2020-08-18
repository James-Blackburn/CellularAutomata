#include "PlayingState.h"

PlayingState::PlayingState()
{
	Simulator::initialize();
}

void PlayingState::update()
{
	Simulator::update();
	Simulator::swap();
	
	if (glfwGetKey(ResourceManager::mainWindow, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(ResourceManager::mainWindow, true);
}

void PlayingState::draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	Simulator::draw();
}