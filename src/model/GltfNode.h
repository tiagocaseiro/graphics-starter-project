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
    static std::shared_ptr<GltfNode> createRoot(const int nodeNum, const std::shared_ptr<tinygltf::Model>& model);
    friend std::ostream& operator<<(std::ostream& os, const GltfNode& node);

private:
    GltfNode(const GltfNode* const parent, const int nodeNum, const tinygltf::Model& model);
    void calculateLocalTRSMatrix();
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
