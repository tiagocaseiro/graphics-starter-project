#include "GltfNode.h"

#include <glm/gtc/type_ptr.hpp>

#include <tiny_gltf.h>

void printWhitespace(std::ostream& os, const int width)
{
    for(int i = 0; i != width; i++)
    {
        os << " ";
    }
}

GltfNodeShared GltfNode::createNode(const GltfNodeShared parent, const int nodeNum, const tinygltf::Model& model,
                                    const std::vector<int>& nodeToJoint,
                                    const std::vector<glm::mat4>& inverseBindMatrices,
                                    std::vector<glm::mat4>& jointMatrices, std::vector<GltfNodeShared>& nodes)
{
    if(nodeNum == -1)
    {
        return nullptr;
    }

    GltfNodeShared node =
        GltfNodeShared(new GltfNode(parent, nodeNum, model, nodeToJoint, inverseBindMatrices, jointMatrices));

    nodes[nodeNum] = node;

    std::vector<int> childrenNodes = model.nodes[nodeNum].children;

    node->mChildNodes.reserve(childrenNodes.size());
    for(const int childNodeNum : model.nodes[nodeNum].children)
    {
        node->mChildNodes.push_back(GltfNodeShared(
            createNode(node, childNodeNum, model, nodeToJoint, inverseBindMatrices, jointMatrices, nodes)));
    }

    return node;
}

GltfNodeShared GltfNode::createNodeTree(const int nodeNum, const tinygltf::Model& model,
                                        const std::vector<int>& nodeToJoint,
                                        const std::vector<glm::mat4>& inverseBindMatrices,
                                        std::vector<glm::mat4>& jointMatrices, std::vector<GltfNodeShared>& nodes)
{
    if(nodeNum == -1)
    {
        return nullptr;
    }

    return createNode(nullptr, nodeNum, model, nodeToJoint, inverseBindMatrices, jointMatrices, nodes);
}

void GltfNode::calculateLocalTRSMatrix()
{
    glm::mat4 const sMatrix = glm::scale(glm::mat4(1.0f), mScale);
    glm::mat4 const rMatrix = glm::mat4_cast(mRotation);
    glm::mat4 const tMatrix = glm::translate(glm::mat4(1.0f), mTranslation);

    mLocalTRSMatrix = tMatrix * rMatrix * sMatrix;
}

void GltfNode::calculateTreeMatrices(const std::vector<int>& nodeToJoint,
                                     const std::vector<glm::mat4>& inverseBindMatrices, glm::mat4 const& parentMatrix,
                                     std::vector<glm::mat4>& jointMatrices)
{
    mNodeMatrix = parentMatrix * mLocalTRSMatrix;

    const int jointIndex = nodeToJoint[mNodeNum];

    jointMatrices[jointIndex] = mNodeMatrix * inverseBindMatrices[jointIndex];

    for(GltfNodeShared child : mChildNodes)
    {
        child->calculateTreeMatrices(nodeToJoint, inverseBindMatrices, mNodeMatrix, jointMatrices);
    }
}

GltfNode::GltfNode(const GltfNodeShared parent, const int nodeNum, const tinygltf::Model& model,
                   const std::vector<int>& nodeToJoint, const std::vector<glm::mat4>& inverseBindMatrices,
                   std::vector<glm::mat4>& jointMatrices)
    : mNodeNum(nodeNum), mNodeName(model.nodes[nodeNum].name)
{
    const tinygltf::Node& node = model.nodes[nodeNum];

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

    calculateLocalTRSMatrix();

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
