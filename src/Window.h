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
    GLFWwindow* mWindow = nullptr;
};