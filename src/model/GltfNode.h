#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "GltfCommon.h"
#include "tools/Macros.h"

SHARED_ONLY(GltfNode);

namespace tinygltf
{
    class Model;
}

class GltfNode
{
public:
    static GltfNodeShared createNodeTree(const int nodeNum, const tinygltf::Model& model,
                                         const std::vector<int>& nodeToJoint,
                                         const std::vector<glm::mat4>& inverseBindMatrices,
                                         std::vector<glm::mat4>& mJointMatrices, std::vector<GltfNodeShared>& nodes);
    friend std::ostream& operator<<(std::ostream& os, const GltfNode& node);

    void setRotation(glm::quat const& rotation) { mRotation = rotation; }
    void setTranslation(glm::vec3 const& translation) { mTranslation = translation; }
    void setScale(glm::vec3 const& scale) { mScale = scale; }

    void calculateLocalTRSMatrix();

    void onAnimationFrame();

    void calculateTreeMatrices(const std::vector<int>& nodeToJoint, const std::vector<glm::mat4>& inverseBindMatrices,
                               glm::mat4 const& parentMatrix, std::vector<glm::mat4>& jointMatrices);

private:
    static GltfNodeShared createNode(const std::shared_ptr<GltfNode> parent, const int nodeNum,
                                     const tinygltf::Model& model, const std::vector<int>& nodeToJoint,
                                     const std::vector<glm::mat4>& inverseBindMatrices,
                                     std::vector<glm::mat4>& mJointMatrices, std::vector<GltfNodeShared>& nodes);
    GltfNode(const GltfNodeShared parent, const int nodeNum, const tinygltf::Model& model,
             const std::vector<int>& nodeToJoint, const std::vector<glm::mat4>& inverseBindMatrices,
             std::vector<glm::mat4>& jointMatrices);
    void printNode(std::ostream& os, int depth) const;

    std::vector<GltfNodeShared> mChildNodes;

    const int mNodeNum;

    std::string mNodeName;

    glm::vec3 mScale       = DEFAULT_SCALE;
    glm::vec3 mTranslation = DEFAULT_TRANSLATION;
    glm::quat mRotation    = DEFAULT_ROTATION;

    glm::mat4 mLocalTRSMatrix = glm::mat4(1.0f);
    glm::mat4 mNodeMatrix     = glm::mat4(1.0f);
};

std::ostream& operator<<(std::ostream& os, const GltfNode& node);
