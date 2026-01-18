#pragma once

#include "FrameBuffer.h"
#include "OGLRenderData.h"
#include "Shader.h"
#include "Texture.h"
#include "UniformBuffer.h"
#include "UserInterface.h"
#include "VertexBuffer.h"
#include "tools/Camera.h"
#include "tools/Timer.h"

struct GLFWWindow;
class GltfModel;

class OGLRenderer
{
public:
    static std::shared_ptr<OGLRenderer> make(const int width, const int height, GLFWwindow* window);

    ~OGLRenderer();

    void setSize(const int width, const int height);
    void uploadData(const OGLMesh& vertexData);
    void draw();
    void handleKeyEvents(const int key, const int scancode, const int action, const int mods);
    void handleMouseButtonEvents(const int button, const int action, const int mods);
    void handleMousePositionEvents(const double xPos, const double yPos);
    void handleMovementKeys();

private:
    OGLRenderer(const Shader& mGltfShader, const Framebuffer& mFramebuffer, const std::shared_ptr<GltfModel>& gltfModel,
                const OGLRenderData& mRenderData);

    Shader mGltfShader;
    Framebuffer mFramebuffer;
    VertexBuffer mVertexBuffer;
    std::shared_ptr<UniformBuffer> m_UniformBuffer;
    std::shared_ptr<UniformBuffer> m_UniformBufferJointMatrices;
    Texture mTex;

    glm::mat4 mViewMatrix       = glm::mat4(1.0);
    glm::mat4 mProjectionMatrix = glm::mat4(1.0);

    std::shared_ptr<GltfModel> mGltfModel;

    OGLRenderData mRenderData;
    UserInterface mUserInterface;
    Timer mUIGenerateTimer;
    Camera mCamera;

    bool mMouseLock     = false;
    int mMouseXPos      = 0;
    int mMouseYPos      = 0;
    float mLastTickTime = 0;
};