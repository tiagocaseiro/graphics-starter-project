#include "GltfAnimationClip.h"

void GltfAnimationClip::addChannel(const tinygltf::Model& model, const tinygltf::Animation& anim,
                                   const tinygltf::AnimationChannel& channel)
{
    mChannels.push_back(GltfAnimationChannel::make(model, anim, channel));
}

void GltfAnimationClip::setAnimationFrame(std::vector<GltfNodeShared> const& nodes, float time)
{
    for(GltfAnimationChannelShared const& channel : mChannels)
    {
        if(channel == nullptr)
        {
            continue;
        }

        int const targetNodeIndex = channel->mTargetNode;

        if(targetNodeIndex >= nodes.size())
        {
            continue;
        }

        GltfNodeShared targetNode = nodes[targetNodeIndex];

        if(targetNode == nullptr)
        {
            continue;
        }

        switch(channel->mTargetPath)
        {
            case ETargetPath::ROTATION:
            {
                targetNode->setRotation(channel->getRotation(time));
                break;
            }
            case ETargetPath::TRANSLATION:
            {
                targetNode->setTranslation(channel->getTranslation(time));
                break;
            }
            case ETargetPath::SCALE:
            {
                targetNode->setScale(channel->getScaling(time));
                break;
            }

            default:
                break;
        }
    }

    for(GltfNodeShared const& node : nodes)
    {
        if(node)
        {
            node->calculateLocalTRSMatrix();
        }
    }
}

GltfAnimationClip::GltfAnimationClip(const std::string& name) : mName(name) {}