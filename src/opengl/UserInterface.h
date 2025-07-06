#pragma once

#include "OGLRenderData.h"

class UserInterface
{
public:
    void init(const OGLRenderData& renderData);
    void createFrame(const OGLRenderData& renderData);
    void render();
    void cleanup();

private:
    float framesPerSecond = 0.0f;
    float averagingAlpha  = 0.96f;
};