#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <iostream>
#include <ctime>

#include "GameState.h"
#include "PlayingState.h"
#include "ResourceManager.h"

int bufferWidth, bufferHeight;
GameState* currentState = nullptr;
bool isFullscreen = true;
int windowedWidth = 1280;
int windowedHeight = 720;
int windowedPosX = 100;
int windowedPosY = 100;

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

    // Create a borderless window sized to the desktop when starting in fullscreen.
    if (isFullscreen)
    {
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        ResourceManager::WINDOW_X = videomode->width;
        ResourceManager::WINDOW_Y = videomode->height;
        ResourceManager::mainWindow = glfwCreateWindow(ResourceManager::WINDOW_X, ResourceManager::WINDOW_Y, "Test Window", NULL, NULL);
        if (ResourceManager::mainWindow)
            glfwSetWindowPos(ResourceManager::mainWindow, 0, 0);
    }
    else
    {
        // Windowed mode
        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
        ResourceManager::WINDOW_X = windowedWidth;
        ResourceManager::WINDOW_Y = windowedHeight;
        ResourceManager::mainWindow = glfwCreateWindow(ResourceManager::WINDOW_X, ResourceManager::WINDOW_Y, "Test Window", NULL, NULL);
        if (ResourceManager::mainWindow)
            glfwSetWindowPos(ResourceManager::mainWindow, windowedPosX, windowedPosY);
    }
    if (!ResourceManager::mainWindow)
    {
        std::cout << "GLFW window creation failed" << std::endl;
        glfwTerminate();
        return 1;
    }
    
    glfwSetInputMode(ResourceManager::mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    glfwMakeContextCurrent(ResourceManager::mainWindow);
    glfwGetFramebufferSize(ResourceManager::mainWindow, &bufferWidth, &bufferHeight);
    glfwSwapInterval(1);
   
    // load OpenGL function pointers with glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize glad.");
    } else {
        // No action needed
    }
    
    glViewport(0, 0, bufferWidth, bufferHeight);
    
    return 0;
}

void toggleFullscreen()
{
    isFullscreen = !isFullscreen;
    
    if (isFullscreen)
    {
        // Save current windowed position and size
        glfwGetWindowPos(ResourceManager::mainWindow, &windowedPosX, &windowedPosY);
        glfwGetWindowSize(ResourceManager::mainWindow, &windowedWidth, &windowedHeight);

        // Borderless fullscreen: remove decorations and resize to monitor size
        const GLFWvidmode* videomode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        ResourceManager::WINDOW_X = videomode->width;
        ResourceManager::WINDOW_Y = videomode->height;
        glfwSetWindowAttrib(ResourceManager::mainWindow, GLFW_DECORATED, GLFW_FALSE);
        glfwSetWindowSize(ResourceManager::mainWindow, ResourceManager::WINDOW_X, ResourceManager::WINDOW_Y);
        glfwSetWindowPos(ResourceManager::mainWindow, 0, 0);
        glfwSetInputMode(ResourceManager::mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else
    {
        // Restore windowed mode: re-enable decorations and restore position/size
        ResourceManager::WINDOW_X = windowedWidth;
        ResourceManager::WINDOW_Y = windowedHeight;
        glfwSetWindowAttrib(ResourceManager::mainWindow, GLFW_DECORATED, GLFW_TRUE);
        glfwSetWindowSize(ResourceManager::mainWindow, windowedWidth, windowedHeight);
        glfwSetWindowPos(ResourceManager::mainWindow, windowedPosX, windowedPosY);
        glfwSetInputMode(ResourceManager::mainWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    
    // Update framebuffer size
    glfwGetFramebufferSize(ResourceManager::mainWindow, &bufferWidth, &bufferHeight);
    glViewport(0, 0, bufferWidth, bufferHeight);
    
    // Reinitialize simulator with new dimensions
    if (currentState)
    {
        Simulator::resize();
    }
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    if (width > 0 && height > 0)
    {
        ResourceManager::WINDOW_X = width;
        ResourceManager::WINDOW_Y = height;
        glViewport(0, 0, width, height);
        
        if (currentState)
        {
            Simulator::resize();
        }
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_F11 && action == GLFW_PRESS)
    {
        toggleFullscreen();
    }
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
    
    // Set up callbacks
    glfwSetFramebufferSizeCallback(ResourceManager::mainWindow, framebufferSizeCallback);
    glfwSetKeyCallback(ResourceManager::mainWindow, keyCallback);
    
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
