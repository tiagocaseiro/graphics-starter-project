#pragma once

#include <memory>
#include <string>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>

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

    std::vector<int> mNodeToJoint;

    std::vector<glm::u16vec4> mJoints;
    std::vector<glm::vec4> mWeights;

    std::vector<glm::mat4> mInverseBindMatrices;
    std::vector<glm::mat4> mJointMatrices;

    std::vector<glm::vec3> mAlteredPositions;

    std::vector<GLuint> mVertexVBO;

    GLuint mVAO      = 0;
    GLuint mIndexVBO = 0;

    Texture mTex;
};