#pragma once

#include <memory>

#include <glm/gtc/type_ptr.hpp>
#include <tiny_gltf.h>

#include "GltfAnimationChannel.h"
#include "GltfNode.h"

class GltfAnimationClip
{
public:
    void addChannel(const tinygltf::Model& model, const tinygltf::Animation& anim,
                    const tinygltf::AnimationChannel& channel);
    void setAnimationFrame(std::vector<GltfNodeShared> const& nodes, float time);

private:
    GltfAnimationClip(const std::string& name);

    const std::string mName;
    std::vector<GltfAnimationChannelShared> mChannels;
};