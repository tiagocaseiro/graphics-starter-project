#pragma once

#include <memory>

#include <glm/gtc/type_ptr.hpp>

#include "GltfAnimationChannel.h"
#include "GltfNode.h"

class GltfAnimationClip
{
public:
    static GltfAnimationClip make(tinygltf::Model const& model, const tinygltf::Animation& animation);

    float getClipEndTime() const;

    void setAnimationFrame(std::vector<GltfNodeShared> const& nodes, float time);

    void playAnimation(int animNum, float speedDivider);

private:
    GltfAnimationClip(std::string const& name, std::vector<GltfAnimationChannelShared> const& channels);

    const std::string mName;
    const std::vector<GltfAnimationChannelShared> mChannels;
};