#include "Model.h"

void Model::init()
{
    mVertexData.vertices = std::vector<OGLVertexData>(6);

    mVertexData.vertices[0].position = {-0.5, -0.5, 0.5};
    mVertexData.vertices[1].position = {0.5, 0.5, 0.5};
    mVertexData.vertices[2].position = {-0.5, 0.5, 0.5};
    mVertexData.vertices[3].position = {-0.5, -0.5, 0.5};
    mVertexData.vertices[4].position = {0.5, -0.5, 0.5};
    mVertexData.vertices[5].position = {0.5, 0.5, 0.5};

    mVertexData.vertices[0].color = {0.0, 0.0, 1.0};
    mVertexData.vertices[1].color = {0.0, 1.0, 1.0};
    mVertexData.vertices[2].color = {1.0, 1.0, 0.0};
    mVertexData.vertices[3].color = {1.0, 0.0, 1.0};
    mVertexData.vertices[4].color = {0.0, 1.0, 0.0};
    mVertexData.vertices[5].color = {1.0, 1.0, 1.0};

    mVertexData.vertices[0].uv = {0.0, 0.0};
    mVertexData.vertices[1].uv = {1.0, 1.0};
    mVertexData.vertices[2].uv = {0.0, 1.0};
    mVertexData.vertices[3].uv = {0.0, 0.0};
    mVertexData.vertices[4].uv = {1.0, 0.0};
    mVertexData.vertices[5].uv = {1.0, 1.0};
}

const OGLMesh& Model::getVertexData()
{
    return mVertexData;
}
