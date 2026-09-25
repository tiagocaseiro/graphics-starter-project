#include "GltfAnimationClip.h"

#include "GltfAnimationChannel.h"

void GltfAnimationClip::addChannel(const tinygltf::Model& model, const tinygltf::Animation& anim,
                                   const tinygltf::AnimationChannel& channel)
{
    mChannels.push_back(GltfAnimationChannel::make(model, anim, channel));
}

GltfAnimationClip::GltfAnimationClip(const std::string& name) : mName(name) {}