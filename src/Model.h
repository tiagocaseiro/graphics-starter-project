#pragma once

#include "opengl/OGLRenderData.h"

class Model
{
public:
    void init();
    const OGLMesh& getVertexData();

private:
    OGLMesh mVertexData;
};