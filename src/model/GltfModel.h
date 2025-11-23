#pragma once

#include <memory>
#include <string>
#include <vector>

#include "opengl/Texture.h"

class OGLRenderData;
class GltfNode;

namespace tinygltf
{
    class Model;
}

class GltfModel
{
public:
    bool loadModel(OGLRenderData& renderData, const std::string& modelFilename, const std::string& textureFilename);
    void draw();
    void cleanup();
    void uploadVertexBuffers();
    void uploadIndexBuffer();

private:
    void createVertexBuffers();
    void createIndexBuffer();

    int getTriangleCount() const;

    std::shared_ptr<tinygltf::Model> mModel;
    std::shared_ptr<GltfNode> mRootNode;
    GLuint mVAO = 0;
    std::vector<GLuint> mVertexVBO;
    GLuint mIndexVBO = 0;
    Texture mTex;
};