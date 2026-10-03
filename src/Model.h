#pragma once

#include "opengl/RenderData.h"

class Model
{
public:
    void init();
    const OGLMesh& getVertexData();

private:
    OGLMesh mVertexData;
};