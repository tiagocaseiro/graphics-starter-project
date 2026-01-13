#include "GltfNode.h"

#include <glm/glm/gtc/type_ptr.hpp>

#include <tinygltf/tiny_gltf.h>

void printWhitespace(std::ostream& os, const int width)
{
    for(int i = 0; i != width; i++)
    {
        os << " ";
    }
}

std::shared_ptr<GltfNode> GltfNode::createNode(const std::shared_ptr<GltfNode> parent, const int nodeNum,
                                               const tinygltf::Model& model, const std::vector<int>& nodeToJoint,
                                               const std::vector<glm::mat4>& inverseBindMatrices,
                                               std::vector<glm::mat4>& jointMatrices)
{
    if(nodeNum == -1)
    {
        return nullptr;
    }

    std::shared_ptr<GltfNode> node = std::shared_ptr<GltfNode>(
        new GltfNode(parent, nodeNum, model, nodeToJoint, inverseBindMatrices, jointMatrices));

    std::vector<int> childrenNodes = model.nodes[nodeNum].children;

    node->mChildNodes.reserve(childrenNodes.size());
    for(const int childNodeNum : model.nodes[nodeNum].children)
    {
        node->mChildNodes.push_back(std::shared_ptr<GltfNode>(
            createNode(node, childNodeNum, model, nodeToJoint, inverseBindMatrices, jointMatrices)));
    }

    return node;
}

std::shared_ptr<GltfNode> GltfNode::createNodeTree(const int nodeNum, const tinygltf::Model& model,
                                                   const std::vector<int>& nodeToJoint,
                                                   const std::vector<glm::mat4>& inverseBindMatrices,
                                                   std::vector<glm::mat4>& jointMatrices)
{
    if(nodeNum == -1)
    {
        return nullptr;
    }

    return createNode(nullptr, nodeNum, model, nodeToJoint, inverseBindMatrices, jointMatrices);
}

GltfNode::GltfNode(const std::shared_ptr<GltfNode> parent, const int nodeNum, const tinygltf::Model& model,
                   const std::vector<int>& nodeToJoint, const std::vector<glm::mat4>& inverseBindMatrices,
                   std::vector<glm::mat4>& jointMatrices)
    : mNodeNum(nodeNum)
{
    const tinygltf::Node& node = model.nodes[nodeNum];

    mNodeName = node.name;
    if(node.scale.empty() == false)
    {
        mScale = glm::make_vec3(node.scale.data());
    }

    if(node.rotation.empty() == false)
    {
        mRotation = glm::make_quat(node.rotation.data());
    }

    if(node.translation.empty() == false)
    {
        mTranslation = glm::make_vec3(node.translation.data());
    }

    const glm::mat4 sMatrix = glm::scale(glm::mat4(1.0f), mScale);
    const glm::mat4 rMatrix = glm::mat4_cast(mRotation);
    const glm::mat4 tMatrix = glm::translate(glm::mat4(1.0f), mTranslation);

    mLocalTRSMatrix = tMatrix * rMatrix * sMatrix;

    const glm::mat4 parentNodeMatrix = parent ? parent->mNodeMatrix : glm::mat4(1.0f);

    mNodeMatrix = parentNodeMatrix * mLocalTRSMatrix;

    const int jointIndex = nodeToJoint[mNodeNum];

    jointMatrices[jointIndex] = mNodeMatrix * inverseBindMatrices[jointIndex];
}

void GltfNode::printNode(std::ostream& os, int depth) const
{
    printWhitespace(os, depth);

    os << mNodeNum << " " << mNodeName << std::endl;

    depth++;

    for(const auto& childNode : mChildNodes)
    {
        if(childNode == nullptr)
        {
            printWhitespace(os, depth);
            os << "null" << std::endl;
            continue;
        }
        childNode->printNode(os, depth);
    }
}

std::ostream& operator<<(std::ostream& os, const GltfNode& node)
{
    node.printNode(os, 0);
    return os;
}
