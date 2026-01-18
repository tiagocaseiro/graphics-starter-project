#pragma once

#include "OGLRenderData.h"

class UserInterface
{
public:
    UserInterface(const OGLRenderData& renderData);
    ~UserInterface();
    void createFrame(const OGLRenderData& renderData);
    void render();

private:
    float framesPerSecond = 0.0f;
    float averagingAlpha  = 0.96f;
};