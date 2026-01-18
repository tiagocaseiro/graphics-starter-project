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
    static std::shared_ptr<GltfModel> make(OGLRenderData& renderData, const std::string& modelFilename,
                                           const std::string& textureFilename);

    ~GltfModel();
    void draw();
    void uploadVertexBuffers();
    void uploadIndexBuffer();

    const std::vector<glm::mat4>& getJointMatrices() const { return mJointMatrices; }

private:
    GltfModel(const std::shared_ptr<tinygltf::Model>& model, const std::shared_ptr<Texture>& tex,
              OGLRenderData& renderData);

    void createVertexBuffers();
    void createIndexBuffer();

    int getTriangleCount() const;

    std::shared_ptr<tinygltf::Model> mModel;
    std::shared_ptr<GltfNode> mRootNode;

    std::vector<int> mNodeToJoint;

    std::vector<glm::mat4> mInverseBindMatrices;
    std::vector<glm::mat4> mJointMatrices;

    std::vector<GLuint> mVertexVBO;

    std::shared_ptr<Texture> mTex;

    GLuint mVAO      = 0;
    GLuint mIndexVBO = 0;
};