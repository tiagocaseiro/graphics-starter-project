#pragma once

#include <memory>
#include <string>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>

#include "GltfAnimationClip.h"
#include "GltfCommon.h"
#include "opengl/Texture.h"
#include "tools/Macros.h"

SHARED_ONLY(GltfModel)

class OGLRenderData;

namespace tinygltf
{
    class Model;
}

class GltfModel
{
public:
    static GltfModelShared make(OGLRenderData& renderData, const std::string& modelFilename,
                                const std::string& textureFilename);

    ~GltfModel();
    void draw();
    void uploadVertexBuffers();
    void uploadIndexBuffer();

    void playAnimation(int animNum, float speedDivider);

    std::string getClipName(int animNum) const;
    int getClipEndTime(int animNum) const;

    const std::vector<glm::mat4>& getJointMatrices() const { return mJointMatrices; }
    // const std::vector<glm::mat2x4>& getJointDualQuats() const { return mJointDualQuats; }

    void setAnimationFrame(int animNum, float time);

private:
    GltfModel(const std::shared_ptr<tinygltf::Model>& model, const std::shared_ptr<Texture>& tex,
              OGLRenderData& renderData);

    void createVertexBuffers();
    void createIndexBuffer();

    int getTriangleCount() const;

    std::shared_ptr<tinygltf::Model> mModel;
    GltfNodeShared mRootNode;

    std::vector<GltfNodeShared> mNodes;

    std::vector<int> mNodeToJoint;

    // std::vector<glm::mat2x4> mJointDualQuats;

    std::vector<glm::mat4> mInverseBindMatrices;
    std::vector<glm::mat4> mJointMatrices;

    std::vector<GLuint> mVertexVBO;

    std::vector<GltfAnimationClip> mAnimClips;

    TextureShared mTex;

    GLuint mVAO      = 0;
    GLuint mIndexVBO = 0;
};