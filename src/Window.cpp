#include "Window.h"

#include <memory>

#include <GLFW/glfw3.h>

#include "Logger.h"

bool Window::init(const int width, const int height, const std::string& title)
{
    if(glfwInit() == false)
    {
        Logger::log(1, "%s glfwInit() error\n", __FUNCTION__);
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    mWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if(mWindow == nullptr)
    {
        Logger::log(1, "%s: Could not create window\n", __FUNCTION__);
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(mWindow);

    glfwSetWindowUserPointer(mWindow, this);

    glfwSetWindowCloseCallback(mWindow, [](GLFWwindow* window) {
        Window* thisWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
        thisWindow->handleWindowCloseEvents();
    });

    glfwSetMouseButtonCallback(mWindow, [](GLFWwindow* window, int button, int action, int mods) {
        Window* thisWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
        thisWindow->handleMouseButtonEvents(button, action, mods);
    });

    mRenderer = std::make_unique<OGLRenderer>();
    if(mRenderer->init(width, height, mWindow) == false)
    {
        Logger::log(1, "%s: Failed to initialize renderer\n", __FUNCTION__);
        glfwTerminate();
        return false;
    }

    glfwSetWindowUserPointer(mWindow, mRenderer.get());

    glfwSetKeyCallback(mWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        OGLRenderer* mRenderer = static_cast<OGLRenderer*>(glfwGetWindowUserPointer(window));
        // thisWindow->handleKeyEvents(key, scancode, action, mods);
        mRenderer->handleKeyEvents(key, scancode, action, mods);
    });

    glfwSetWindowSizeCallback(mWindow, [](GLFWwindow* window, int width, int height) {
        OGLRenderer* renderer = static_cast<OGLRenderer*>(glfwGetWindowUserPointer(window));
        renderer->setSize(width, height);
    });
    // glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // if(glfwRawMouseMotionSupported())
    // {
    //     glfwSetInputMode(mWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    // }

    mModel = std::make_unique<Model>();
    mModel->init();

    Logger::log(1, "%s: Window successfully initialized\n", __FUNCTION__);
    return true;
}

void Window::mainLoop()
{
    glfwSwapInterval(1);

    mRenderer->uploadData(mModel->getVertexData());

    while(glfwWindowShouldClose(mWindow) == false)
    {
        mRenderer->draw();
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }
}

void Window::cleanup()
{
    Logger::log(1, "%s: Terminating window\n", __FUNCTION__);
    glfwDestroyWindow(mWindow);
    glfwTerminate();
}

void Window::handleWindowCloseEvents()
{
    Logger::log(1, "%s: Window got a close event\n", __FUNCTION__);
}

void Window::handleKeyEvents(const int key, const int scancode, const int action, const int mods)
{
    std::string actionName;

    switch(action)
    {
        case GLFW_RELEASE:
            actionName = "released";
            break;
        case GLFW_PRESS:
            actionName = "press";
            break;
        case GLFW_REPEAT:
            actionName = "repeat";
            break;
        default:
            actionName = "invalid";
            break;
    }

    const char* keyName = glfwGetKeyName(key, 0);

    Logger::log(1, "%s: key %s (key %i, scandcode %i) %s\n", __FUNCTION__, keyName, key, scancode, actionName.c_str());
}

void Window::handleMouseButtonEvents(const int button, const int action, const int mods)
{
    std::string actionName;

    switch(action)
    {
        case GLFW_RELEASE:
            actionName = "released";
            break;
        case GLFW_PRESS:
            actionName = "press";
            break;
        case GLFW_REPEAT:
            actionName = "repeat";
            break;
        default:
            actionName = "invalid";
            break;
    }

    std::string mouseButtonName;

    switch(button)
    {
        case GLFW_MOUSE_BUTTON_LEFT:
            mouseButtonName = "left";
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            mouseButtonName = "middle";
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            mouseButtonName = "right";
            break;
        default:
            mouseButtonName = "other";
            break;
    }

    Logger::log(1, "%s: %s mouse button (%i) %s\n", __FUNCTION__, mouseButtonName.c_str(), button, actionName.c_str());
}
