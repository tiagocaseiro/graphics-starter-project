#pragma once

#include <memory>

#include <glm/gtc/type_ptr.hpp>
#include <tiny_gltf.h>

enum class ETargetPath
{
    ROTATION,
    TRANSLATION,
    SCALE
};

enum class EInterpolationType
{
    STEP,
    LINEAR,
    CUBICSPLINE
};

class GltfAnimationChannel
{
public:
    static std::shared_ptr<GltfAnimationChannel> make(const tinygltf::Model& model, const tinygltf::Animation& anim,
                                                      const tinygltf::AnimationChannel& channel);
    int getTargetNode() const;
    ETargetPath getTargetPath() const;

    glm::quat getRotation(float time);
    glm::vec3 getTranslation(float time);
    glm::vec3 getScaling(float time);

    float getMaxTime() const;

private:
    GltfAnimationChannel(int targetNode, const std::vector<float>& Timings, EInterpolationType interType,
                         const ETargetPath targetPath, const std::vector<glm::quat>& rotations,
                         const std::vector<glm::vec3>& translations, const std::vector<glm::vec3>& scaling);

    void SetTimings(const std::vector<float>& timings);
    void SetScaling(const std::vector<glm::vec3>& scaling);
    void SetTranslations(const std::vector<glm::vec3>& translation);
    void SetRotations(const std::vector<glm::vec3>& rotation);

    const int mTargetNode;

    const EInterpolationType mInterType;
    const ETargetPath mTargetPath;

    const std::vector<float> mTimings;
    const std::vector<glm::quat> mRotations;
    const std::vector<glm::vec3> mTranslations;
    const std::vector<glm::vec3> mScaling;
};