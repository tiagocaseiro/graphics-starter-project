#include "GltfAnimationChannel.h"

template <typename T>
static std::vector<T> initializeTransformations(const tinygltf::Accessor& outputAccessor,
                                                const tinygltf::BufferView& outputBufferView,
                                                const tinygltf::Buffer& outputBuffer)
{
    std::vector<T> result;
    result.reserve(outputAccessor.count);

    std::memcpy(result.data(), &outputBuffer.data.at(0) + outputBufferView.byteOffset, outputBufferView.byteLength);

    return result;
}

template <ETargetPath targetPath, typename T>
static T getTransformForTime(std::vector<T> const& transforms, std::vector<float> const& timings,
                             EInterpolationType const interType, float const time)
{
    if(transforms.empty())
    {
        return glm::vec3();
    }

    if(time < timings.front())
    {
        return transforms.front();
    }

    if(time > timings.back())
    {
        return transforms.back();
    }

    int nextTimeIndex = 0;
    for(int i = 0; i != timings.size(); i++)
    {
        if(timings[i] > time)
        {
            nextTimeIndex = i;
            break;
        }
    }

    int previousTimeIndex = std::max(0, nextTimeIndex - 1);

    if(previousTimeIndex == nextTimeIndex)
    {
        return transforms.at(previousTimeIndex);
    }

    T finalTransform;
    if constexpr(targetPath == ETargetPath::SCALE)
    {
        finalTransform = glm::vec3(1.0f);
    }
    if constexpr(targetPath == ETargetPath::TRANSLATION)
    {
        finalTransform = glm::vec3(0.0f);
    }
    if constexpr(targetPath == ETargetPath::ROTATION)
    {
        finalTransform = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    }

    switch(interType)
    {
        case EInterpolationType::STEP:
        {
            finalTransform = transforms[previousTimeIndex];
            break;
        }

        case EInterpolationType::LINEAR:
        {
            float const interpolatedTime =
                (time - timings[previousTimeIndex]) / (timings[nextTimeIndex] - timings[previousTimeIndex]);

            T const previousScale = transforms[previousTimeIndex];
            T const nextScale     = transforms[nextTimeIndex];

            finalTransform = previousScale + interpolatedTime * (nextScale - previousScale);
            break;
        }

        case EInterpolationType::CUBICSPLINE:
        {
            float const deltaTime = timings[nextTimeIndex] - timings[previousTimeIndex];

            T const prevTangent = deltaTime * transforms[previousTimeIndex * 3 + 2];
            T const nextTangent = deltaTime * transforms[nextTimeIndex * 3];

            float const interpolatedTime =
                (time - timings[previousTimeIndex]) / (timings[nextTimeIndex] - timings[previousTimeIndex]);

            float const interpolatedTimeSq  = interpolatedTime * interpolatedTime;
            float const interpolatedTimeCub = interpolatedTimeSq * interpolatedTime;

            T const prevPoint = transforms[previousTimeIndex * 3 + 1];
            T const nextPoint = transforms[nextTimeIndex * 3 + 1];

            // LEARN: What does this mean?
            finalTransform = (2 * interpolatedTimeCub - 3 * interpolatedTimeSq + 1) * prevPoint +
                             (interpolatedTimeCub - 2 * interpolatedTimeSq + interpolatedTime) * prevTangent +
                             (-2 * interpolatedTimeCub + 3 * interpolatedTimeSq) * nextPoint +
                             (interpolatedTimeCub - interpolatedTimeSq) * nextTangent;

            break;
        }
        default:
            break;
    }
    return finalTransform;
}

GltfAnimationChannelShared GltfAnimationChannel::make(const tinygltf::Model& model, const tinygltf::Animation& anim,
                                                      const tinygltf::AnimationChannel& channel)
{
    const int targetNode = channel.target_node;

    const tinygltf::Accessor& inputAccessor     = model.accessors.at(anim.samplers.at(channel.sampler).input);
    const tinygltf::BufferView& inputBufferView = model.bufferViews.at(inputAccessor.bufferView);
    const tinygltf::Buffer& inputBuffer         = model.buffers.at(inputBufferView.buffer);

    std::vector<float> timings;
    timings.resize(inputAccessor.count);
    std::memcpy(timings.data(), &inputBuffer.data.at(0) + inputBufferView.byteOffset, inputBufferView.byteLength);

    const tinygltf::AnimationSampler sampler = anim.samplers.at(channel.sampler);

    EInterpolationType interType = EInterpolationType::STEP;
    if(sampler.interpolation == "STEP")
    {
        interType = EInterpolationType::STEP;
    }
    else if(sampler.interpolation == "LINEAR")
    {
        interType = EInterpolationType::LINEAR;
    }
    else
    {
        interType = EInterpolationType::CUBICSPLINE;
    }

    const tinygltf::Accessor& outputAccessor     = model.accessors.at(anim.samplers.at(channel.sampler).output);
    const tinygltf::BufferView& outputBufferView = model.bufferViews.at(outputAccessor.bufferView);
    const tinygltf::Buffer& outputBuffer         = model.buffers.at(outputBufferView.buffer);

    ETargetPath targetPath = ETargetPath::ROTATION;
    std::vector<glm::quat> rotations;
    std::vector<glm::vec3> translations;
    std::vector<glm::vec3> scaling;

    if(channel.target_path == "rotation")
    {
        targetPath = ETargetPath::ROTATION;
        rotations  = initializeTransformations<glm::quat>(outputAccessor, outputBufferView, outputBuffer);
    }
    else if(channel.target_path == "translation")
    {
        targetPath   = ETargetPath::TRANSLATION;
        translations = initializeTransformations<glm::vec3>(outputAccessor, outputBufferView, outputBuffer);
    }
    else
    {
        targetPath = ETargetPath::SCALE;
        scaling    = initializeTransformations<glm::vec3>(outputAccessor, outputBufferView, outputBuffer);
    }

    return GltfAnimationChannelShared(
        new GltfAnimationChannel(targetNode, timings, interType, targetPath, rotations, translations, scaling));
}

glm::vec3 GltfAnimationChannel::getScaling(float time) const
{
    return getTransformForTime<ETargetPath::SCALE>(mScaling, mTimings, mInterType, time);
}

glm::vec3 GltfAnimationChannel::getTranslation(float time) const
{
    return getTransformForTime<ETargetPath::TRANSLATION>(mTranslations, mTimings, mInterType, time);
}

glm::quat GltfAnimationChannel::getRotation(float time) const
{
    return getTransformForTime<ETargetPath::ROTATION>(mRotations, mTimings, mInterType, time);
}

GltfAnimationChannel::GltfAnimationChannel(int targetNode, const std::vector<float>& timings,
                                           EInterpolationType interType, const ETargetPath targetPath,
                                           const std::vector<glm::quat>& rotations,
                                           const std::vector<glm::vec3>& translations,
                                           const std::vector<glm::vec3>& scaling)
    : mTargetNode(targetNode),
      mTimings(timings),
      mInterType(interType),
      mTargetPath(targetPath),
      mRotations(rotations),
      mTranslations(translations),
      mScaling(scaling)
{
}
