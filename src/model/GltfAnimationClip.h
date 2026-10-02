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

    void blendAnimationFrame(std::vector<GltfNodeShared> const& nodes, float time, float blendFactor);

    void playAnimation(int animNum, float speedDivider);

private:
    GltfAnimationClip(std::string const& name, std::vector<GltfAnimationChannelShared> const& channels);

    const std::vector<GltfAnimationChannelShared> mChannels;

public:
    const std::string mName;
};