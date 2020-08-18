#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <ctime>

#include "GameState.h"
#include "PlayingState.h"
#include "ResourceManager.h"

int bufferWidth, bufferHeight;
GameState* currentState = nullptr;

int createWindow()
{
    if (!glfwInit())
    {
        std::cout << "GLFW initialisation failed" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    const GLFWvidmode* videomode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    ResourceManager::WINDOW_X = videomode->width;
    ResourceManager::WINDOW_Y = videomode->height;
    ResourceManager::mainWindow = glfwCreateWindow(ResourceManager::WINDOW_X, ResourceManager::WINDOW_Y, "Test Window", glfwGetPrimaryMonitor(), NULL);
    if (!ResourceManager::mainWindow)
    {
        std::cout << "GLFW window creation failed" << std::endl;
        glfwTerminate();
        return 1;
    }
    
    glfwGetFramebufferSize(ResourceManager::mainWindow, &bufferWidth, &bufferHeight);
    glfwSetInputMode(ResourceManager::mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwMakeContextCurrent(ResourceManager::mainWindow);
    glfwSwapInterval(1);
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW initialisation failed" << std::endl;
        glfwDestroyWindow(ResourceManager::mainWindow);
        glfwTerminate();
        return 1;
    }
    glViewport(0, 0, bufferWidth, bufferHeight);
    
    return 0;
}

int main()
{
    srand(time(NULL));
    
    int automatonType;
    const char* msg = 
    {
        "Select a Cellular Automaton type\n"
        "Conways Game Of Life   [1]\n"
        "Brian's Brain          [2]\n"
        "Day And Night          [3]\n"
        ">> "
    };
    
    std::cout << msg;
    std::cin >> automatonType;
    Simulator::setAutomatonType(automatonType - 1);

    // Setup window
    if (createWindow())
    {
        std::cout << "Error creating window" << std::endl;
        return 1;
    }

    // load, compile and link shaders
    ResourceManager::setupShaders();
    // set current state to playing state
    currentState = new PlayingState;
    while (!glfwWindowShouldClose(ResourceManager::mainWindow))
    {
        glfwPollEvents();
        
        currentState->update();
        currentState->draw();

        glfwSwapBuffers(ResourceManager::mainWindow);
    }
    
    // Cleanup
    delete currentState;
    glfwTerminate();
    return 0;
}
