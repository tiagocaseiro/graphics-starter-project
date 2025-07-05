#pragma once

#include "OGLRenderData.h"

class UserInterface
{
public:
    void init(const OGLRenderData& renderData);
    void createFrame(const OGLRenderData& renderData);
    void render();
    void cleanup();
};