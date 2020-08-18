#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GameState.h"
#include "Simulator.h"

class PlayingState : public GameState
{
public:
	PlayingState();

	void update();
	void draw();
};

