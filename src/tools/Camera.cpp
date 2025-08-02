#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Camera::getViewMatrix(const OGLRenderData& renderData)
{
    const float azimRad = glm::radians(renderData.rdViewAzimuth);
    const float elevRad = glm::radians(renderData.rdViewElevation);

    const float sinAzim = glm::sin(azimRad);
    const float cosAzim = glm::cos(azimRad);
    const float sinElev = glm::sin(elevRad);
    const float cosElev = glm::cos(elevRad);

    mViewDirection = glm::normalize(glm::vec3(sinAzim * cosElev, sinElev, -cosAzim * cosElev));
    return glm::lookAt(mWorldPos, mWorldPos + mViewDirection, mWorldUpVector);
}
