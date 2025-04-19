#pragma once

#include <string>

struct GLFWwindow;

class Window
{
public:
    bool init(const int width, const int height, const std::string& title);
    void mainLoop();
    void cleanup();

private:
    void handleWindowCloseEvents();
    void handleKeyEvents(const int key, const int scancode, const int action, const int mods);
    void handleMouseButtonEvents(const int button, const int action, const int mods);

    GLFWwindow* mWindow = nullptr;
};