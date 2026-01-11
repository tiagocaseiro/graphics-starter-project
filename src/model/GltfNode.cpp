#include "GltfNode.h"

#include <tinygltf/tiny_gltf.h>

void printWhitespace(std::ostream& os, const int width)
{
    for(int i = 0; i != width; i++)
    {
        os << " ";
    }
}

std::shared_ptr<GltfNode> GltfNode::createRoot(const int nodeNum, const tinygltf::Model& model,
                                               const std::vector<int>& nodeToJoint,
                                               const std::vector<glm::mat4>& inverseBindMatrices,
                                               std::vector<glm::mat4>& jointMatrices)
{
    if(nodeNum == -1)
    {
        return nullptr;
    }

    return std::shared_ptr<GltfNode>(
        new GltfNode(nullptr, nodeNum, model, nodeToJoint, inverseBindMatrices, jointMatrices));
}

GltfNode::GltfNode(const GltfNode* const parent, const int nodeNum, const tinygltf::Model& model,
                   const std::vector<int>& nodeToJoint, const std::vector<glm::mat4>& inverseBindMatrices,
                   std::vector<glm::mat4>& jointMatrices)
    : mNodeNum(nodeNum)
{
    const tinygltf::Node& node = model.nodes[nodeNum];

    mNodeName = node.name;
    if(node.scale.empty() == false)
    {
        mScale = glm::vec3(node.scale[0], node.scale[1], node.scale[2]);
    }

    if(node.rotation.empty() == false)
    {
        mRotation = glm::quat(node.rotation[0], node.rotation[1], node.rotation[2], node.rotation[3]);
    }

    if(node.translation.empty() == false)
    {
        mTranslation = glm::vec3(node.translation[0], node.translation[1], node.translation[2]);
    }

    calculateLocalTRSMatrix();

    glm::mat4 parentNodeMatrix = parent ? parent->mNodeMatrix : glm::mat4(1.0f);

    mNodeMatrix = parentNodeMatrix * mLocalTRSMatrix;

    const int jointIndex = nodeToJoint[mNodeNum];

    jointMatrices[jointIndex] = mNodeMatrix * inverseBindMatrices[jointIndex];
    mChildNodes.reserve(node.children.size());
    for(const int childNodeNum : node.children)
    {
        mChildNodes.push_back(std::shared_ptr<GltfNode>(
            new GltfNode(this, childNodeNum, model, nodeToJoint, inverseBindMatrices, jointMatrices)));
    }
}

void GltfNode::calculateLocalTRSMatrix()
{
    glm::mat4 sMatrix = glm::scale(glm::mat4(1.0f), mScale);
    glm::mat4 rMatrix = glm::mat4_cast(mRotation);
    glm::mat4 tMatrix = glm::translate(glm::mat4(1.0f), mTranslation);

    mLocalTRSMatrix = tMatrix * rMatrix * sMatrix;
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
