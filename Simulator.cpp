#include "Simulator.h"

Shader* Simulator::cellularShader = nullptr;
GLuint Simulator::frontTexture = 0;
GLuint Simulator::backTexture = 0;
GLuint Simulator::framebuffer = 0;
GLuint Simulator::fullscreenVAO = 0;
GLuint Simulator::fullscreenVBO = 0;
GLuint Simulator::placementVAO = 0;
GLuint Simulator::placementVBO = 0;

GLuint Simulator::uniformSimulatorVPlocation=  0;
GLuint Simulator::uniformCopyVPlocation = 0;
GLuint Simulator::uniformPlacementVPlocation = 0;
GLuint Simulator::uniformPlacementMousePoslocation = 0;
GLuint Simulator::uniformPlacementRadiusLocation = 0;
GLuint Simulator::uniformNoiseVPlocation = 0;
GLuint Simulator::uniformSimulatorScaleLocation = 0;
GLuint Simulator::uniformCopyScaleLocation = 0;

glm::mat4 Simulator::projection;
glm::mat4 Simulator::vp;
double Simulator::cursorSize = 10.0f;

void Simulator::initialize()
{
	frontTexture = createTexture();
	backTexture = createTexture();

	GLfloat fullscreenVertices[12] = {
		0, 0, 0,
		0, ResourceManager::WINDOW_Y, 0,
		ResourceManager::WINDOW_X, 0, 0,
		ResourceManager::WINDOW_X, ResourceManager::WINDOW_Y, 0,
	};

	// get uniform locations
	uniformSimulatorVPlocation = glGetUniformLocation(cellularShader->getID(), "viewProjection");
	uniformCopyVPlocation = glGetUniformLocation(ResourceManager::copyShader.getID(), "viewProjection");
	uniformPlacementVPlocation = glGetUniformLocation(ResourceManager::placementShader.getID(), "viewProjection");
	uniformPlacementMousePoslocation = glGetUniformLocation(ResourceManager::placementShader.getID(), "mousePos");
	uniformPlacementRadiusLocation = glGetUniformLocation(ResourceManager::placementShader.getID(), "circleRadius");
	uniformNoiseVPlocation = glGetUniformLocation(ResourceManager::noiseShader.getID(), "viewProjection");
	uniformSimulatorScaleLocation = glGetUniformLocation(cellularShader->getID(), "scale");
	uniformCopyScaleLocation = glGetUniformLocation(ResourceManager::copyShader.getID(), "scale");
	projection = glm::ortho(0.0f, (float)ResourceManager::WINDOW_X, 0.0f, (float)ResourceManager::WINDOW_Y, 0.1f, 1000.0f);
	vp = projection * glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// generate fullscreen mesh
	glGenFramebuffers(1, &framebuffer);
	glGenVertexArrays(1, &fullscreenVAO);
	glBindVertexArray(fullscreenVAO);
	glGenBuffers(1, &fullscreenVBO);
	glBindBuffer(GL_ARRAY_BUFFER, fullscreenVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fullscreenVertices), fullscreenVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, GL_NONE);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	glBindVertexArray(GL_NONE);

	// generate placement circle mesh
	glGenVertexArrays(1, &placementVAO);
	glBindVertexArray(placementVAO);
	glGenBuffers(1, &placementVBO);
	glBindBuffer(GL_ARRAY_BUFFER, placementVBO);
	
	// unit circle
	const double circleRadius = (int)cursorSize / 2;
	double angle = 0;
	std::vector<GLfloat> vertices(NUM_POINTS * 3);
	for (int i = 0; i < NUM_POINTS * 3; i += 3) 
	{
		vertices[i] = cos(glm::radians(angle));
		vertices[i + 1] = sin(glm::radians(angle));
		vertices[i + 2] = 0;
		angle += (360.0f / (float)NUM_POINTS);
	}
	
	glBufferData(GL_ARRAY_BUFFER, NUM_POINTS * 3 * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, GL_NONE);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	glBindVertexArray(GL_NONE);
	
	
	glfwSetScrollCallback(ResourceManager::mainWindow, mouseScroll);
	glfwSetCursorPosCallback(ResourceManager::mainWindow, mouseMove);
	populate(frontTexture);
}

void Simulator::setAutomatonType(int type)
{
	switch (type)
	{
	case CONWAYS_GAME_OF_LIFE:
		cellularShader = &ResourceManager::ConwaysGameOfLifeShader;
		break;
	case BRIANS_BRAIN:
		cellularShader = &ResourceManager::BriansBrainShader;
		break;
	case DAY_AND_NIGHT:
		cellularShader = &ResourceManager::DayAndNightShader;
		break;
	default:
		std::cout << "Invalid type selected..." << std::endl;
		std::cout << "Defaulting to Conways Game Of Life" << std::endl;
		cellularShader = &ResourceManager::ConwaysGameOfLifeShader;
		break;
	}
}

GLuint Simulator::createTexture()
{
	GLuint newTexture = 0;
	glGenTextures(1, &newTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, newTexture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		ResourceManager::WINDOW_X,
		ResourceManager::WINDOW_Y,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		GL_NONE
	);
	
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	return newTexture;
}

void Simulator::update()
{
	mouseInput();

	glUseProgram(cellularShader->getID());
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							GL_TEXTURE_2D, backTexture, 0);

	glUniformMatrix4fv(uniformSimulatorVPlocation, 1, GL_FALSE, glm::value_ptr(vp));
	glUniform2f(uniformSimulatorScaleLocation, (float)ResourceManager::WINDOW_X, (float)ResourceManager::WINDOW_Y);

	glBindTexture(GL_TEXTURE_2D, frontTexture);
	glBindVertexArray(fullscreenVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(GL_NONE);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void Simulator::swap()
{
	GLuint tmp = frontTexture;
	frontTexture = backTexture;
	backTexture = tmp;
}

void Simulator::draw()
{
	// Draw texture
	glUseProgram(ResourceManager::copyShader.getID());

	glUniformMatrix4fv(uniformCopyVPlocation, 1, GL_FALSE, glm::value_ptr(vp));
	glUniform2f(uniformCopyScaleLocation, (float)ResourceManager::WINDOW_X, (float)ResourceManager::WINDOW_Y);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, frontTexture);
	glBindVertexArray(fullscreenVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(GL_NONE);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);

	// Draw placement circle
	glUseProgram(ResourceManager::placementShader.getID());
	
	double xpos, ypos;
	glfwGetCursorPos(ResourceManager::mainWindow, &xpos, &ypos);
	
	glUniform1f(uniformPlacementRadiusLocation, cursorSize / 2.0f);
	glUniform2f(uniformPlacementMousePoslocation, xpos, ResourceManager::WINDOW_Y - ypos);
	glUniformMatrix4fv(uniformPlacementVPlocation, 1, GL_FALSE, glm::value_ptr(vp));
	
	glBindVertexArray(placementVAO);
	glDrawArrays(GL_LINE_LOOP, 0, NUM_POINTS);
	glBindVertexArray(GL_NONE);
}

void Simulator::populate(GLuint texture)
{
	glUseProgram(ResourceManager::noiseShader.getID());
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, frontTexture, 0);

	glUniformMatrix4fv(uniformNoiseVPlocation, 1, GL_FALSE, glm::value_ptr(vp));

	glBindVertexArray(fullscreenVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void Simulator::mouseInput()
{
	// Place cells
	int leftButtonState = glfwGetMouseButton(ResourceManager::mainWindow, GLFW_MOUSE_BUTTON_LEFT);
	int rightButtonState = glfwGetMouseButton(ResourceManager::mainWindow, GLFW_MOUSE_BUTTON_RIGHT);
	if (leftButtonState == GLFW_PRESS || rightButtonState == GLFW_PRESS)
	{
		int value;
		double xoffset, yoffset;
		int iCursorSize = (int)cursorSize;
		
		glfwGetCursorPos(ResourceManager::mainWindow, &xoffset, &yoffset);
		xoffset = xoffset - iCursorSize / 2;
		yoffset = ResourceManager::WINDOW_Y - (yoffset + iCursorSize / 2);

		int placementSizeX = iCursorSize, placementSizeY = iCursorSize;

		if (leftButtonState == GLFW_PRESS)
			value = 255;
		else if (rightButtonState == GLFW_PRESS)
			value = 0;

		// read back texture data
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D, frontTexture, 0);
		std::vector<GLubyte> pixelData(iCursorSize * iCursorSize * 4);
		glReadPixels(xoffset, 
			yoffset,
			placementSizeX,
			placementSizeY,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			pixelData.data()
		);
		glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);

		// determine which pixels fall within placement circle
		const double circleRadius = iCursorSize / 2;
		const double circleCentre = iCursorSize / 2;

		for (int x = 0; x < iCursorSize; x++)
		{
			for (int y = 0; y < iCursorSize; y++)
			{
				double distance = sqrt(pow(x - circleCentre, 2) + pow(y - circleCentre, 2));
				if (distance < circleRadius)
				{
					const int transformedIndex = (y * 4 * iCursorSize) + x * 4;
					pixelData[transformedIndex] = value;
					pixelData[transformedIndex + 1] = value;
					pixelData[transformedIndex + 2] = value;
					pixelData[transformedIndex + 3] = value;
				}
			}
		}

		// send modified data back to texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, frontTexture);
		glTexSubImage2D(GL_TEXTURE_2D,
			0,
			xoffset,
			yoffset,
			placementSizeX,
			placementSizeY,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			pixelData.data()
		);
		glBindTexture(GL_TEXTURE_2D, GL_NONE);
	}
}

void Simulator::mouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	cursorSize += yoffset * 4.0f;
	if (cursorSize > MAX_PLACEMENT_SIZE)
		cursorSize = MAX_PLACEMENT_SIZE;
	if (cursorSize < MIN_PLACEMENT_SIZE)
		cursorSize = MIN_PLACEMENT_SIZE;
}

void Simulator::mouseMove(GLFWwindow* window, double xpos, double ypos)
{
	double new_xpos = xpos;
	double new_ypos = ypos;

	if (xpos + cursorSize / 2 > ResourceManager::WINDOW_X)
		new_xpos = ResourceManager::WINDOW_X - cursorSize / 2;
	if (xpos - cursorSize / 2 < 0)
		new_xpos = cursorSize / 2;
	if (ypos + cursorSize / 2 > ResourceManager::WINDOW_Y)
		new_ypos = ResourceManager::WINDOW_Y - cursorSize / 2;
	if (ypos - cursorSize / 2 < 0)
		new_ypos = cursorSize / 2;

	glfwSetCursorPos(ResourceManager::mainWindow, new_xpos, new_ypos);
}