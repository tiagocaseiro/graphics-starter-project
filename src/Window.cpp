#include "Window.h"

#include <GLFW/glfw3.h>

#include "Logger.h"

bool Window::init(const int width, const int height, const std::string& title)
{
    if(glfwInit() == false)
    {
        Logger::log(1, "%s glfwInit() error\n", __FUNCTION__);
        return false;
    }

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    mWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if(mWindow == nullptr)
    {
        Logger::log(1, "Could not create window\n", __FUNCTION__);
        glfwTerminate();
        return false;
    }

    Logger::log(1, "Window successfully initialized\n", __FUNCTION__);
    return true;
}

void Window::mainLoop()
{
    while(glfwWindowShouldClose(mWindow) == false)
    {
        glfwPollEvents();
    }
}

void Window::cleanup()
{
    Logger::log(1, "Terminating window\n", __FUNCTION__);
    glfwDestroyWindow(mWindow);
    glfwTerminate();
}
