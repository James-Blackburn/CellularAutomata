#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ResourceManager.h"

class Simulator
{
public:
	static void initialize();
	static void update();
	static void swap();
	static void draw();
	static void setAutomatonType(int type);

	enum AUTOMATON_TYPES
	{
		CONWAYS_GAME_OF_LIFE, 
		BRIANS_BRAIN,
		DAY_AND_NIGHT,
	};

private:
	Simulator() {};

	static Shader* cellularShader;
	static GLuint frontTexture;
	static GLuint backTexture;
	static GLuint framebuffer;
	static GLuint fullscreenVAO;
	static GLuint fullscreenVBO;
	static GLuint placementVAO;
	static GLuint placementVBO;
	
	static GLuint uniformSimulatorVPlocation;
	static GLuint uniformCopyVPlocation;
	static GLuint uniformPlacementVPlocation;
	static GLuint uniformPlacementMousePoslocation;
	static GLuint uniformPlacementRadiusLocation;
	static GLuint uniformNoiseVPlocation;
	static GLuint uniformSimulatorScaleLocation;
	static GLuint uniformCopyScaleLocation;
	
	static glm::mat4 projection;
	static glm::mat4 vp;
	static double cursorSize;

	static const int MAX_PLACEMENT_SIZE = 400;
	static const int MIN_PLACEMENT_SIZE = 5;
	static const int NUM_POINTS = 24;

	static GLuint createTexture();
	static void populate(GLuint texture);
	static void mouseInput();
	static void mouseScroll(GLFWwindow* window, double xoffset, double yoffset);
	static void mouseMove(GLFWwindow* window, double xpos, double ypos);
};

