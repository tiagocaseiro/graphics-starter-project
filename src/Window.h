#pragma once

#include <memory>
#include <string>

#include "Model.h"
#include "opengl/OGLRenderer.h"

struct GLFWwindow;

class Model;

class Window
{
public:
    bool init(const int width, const int height, const std::string& title);
    void mainLoop();
    void cleanup();

    const OGLRendererShared& getRenderer() { return mRenderer; }

private:
    void handleWindowCloseEvents();
    void handleKeyEvents(const int key, const int scancode, const int action, const int mods);

    GLFWwindow* mWindow = nullptr;

    OGLRendererShared mRenderer;
    std::unique_ptr<Model> mModel;
};