#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace tinygltf
{
    class Model;
}

class GltfNode
{
public:
    static std::shared_ptr<GltfNode> createNodeTree(const int nodeNum, const tinygltf::Model& model,
                                                    const std::vector<int>& nodeToJoint,
                                                    const std::vector<glm::mat4>& inverseBindMatrices,
                                                    std::vector<glm::mat4>& mJointMatrices);
    friend std::ostream& operator<<(std::ostream& os, const GltfNode& node);

private:
    static std::shared_ptr<GltfNode> createNode(const std::shared_ptr<GltfNode> parent, const int nodeNum,
                                                const tinygltf::Model& model, const std::vector<int>& nodeToJoint,
                                                const std::vector<glm::mat4>& inverseBindMatrices,
                                                std::vector<glm::mat4>& mJointMatrices);
    GltfNode(const std::shared_ptr<GltfNode> parent, const int nodeNum, const tinygltf::Model& model,
             const std::vector<int>& nodeToJoint, const std::vector<glm::mat4>& inverseBindMatrices,
             std::vector<glm::mat4>& jointMatrices);
    void printNode(std::ostream& os, int depth) const;

    std::vector<std::shared_ptr<GltfNode>> mChildNodes;

    const int mNodeNum;

    std::string mNodeName;

    glm::vec3 mScale          = glm::vec3(1.0f);
    glm::vec3 mTranslation    = glm::vec3(0.0f);
    glm::quat mRotation       = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::mat4 mLocalTRSMatrix = glm::mat4(1.0f);
    glm::mat4 mNodeMatrix     = glm::mat4(1.0f);
};

std::ostream& operator<<(std::ostream& os, const GltfNode& node);
