#include "OGLRenderer.h"

#include <algorithm>
#include <iostream>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui_impl_glfw.h>

#include <GLFW/glfw3.h>

#include "Logger.h"

#include "Shader.h"
#include "ShaderStorageBuffer.h"
#include "UniformBuffer.h"
#include "model/GltfModel.h"

std::shared_ptr<OGLRenderer> OGLRenderer::make(const int width, const int height, GLFWwindow* window)
{
    if(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == false || GLAD_GL_VERSION_4_6 == false)
    {
        return nullptr;
    }

    Framebuffer framebuffer;
    if(framebuffer.init(width, height) == false)
    {
        return nullptr;
    }

    OGLRenderData renderData;

    renderData.rdWidth  = width;
    renderData.rdHeight = height;
    renderData.rdWindow = window;

    std::shared_ptr<Shader> gltfShader = Shader::make("../shaders/gltf_gpu.vert", "../shaders/gltf_gpu.frag");
    if(gltfShader == nullptr)
    {
        return nullptr;
    }

    static constexpr auto modelFileName    = "../assets/Woman.gltf";
    static constexpr auto modelTexFilename = "../textures/Woman.png";

    std::shared_ptr<GltfModel> gltfModel = GltfModel::make(renderData, modelFileName, modelTexFilename);

    if(gltfModel == nullptr)
    {
        return nullptr;
    }

    gltfModel->uploadIndexBuffer();

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    return std::shared_ptr<OGLRenderer>(new OGLRenderer(gltfShader, framebuffer, gltfModel, renderData));
}

OGLRenderer::OGLRenderer(const std::shared_ptr<Shader>& gltfShader, const Framebuffer& framebuffer,
                         const std::shared_ptr<GltfModel>& gltfModel, const OGLRenderData& renderData)
    : mGltfShader(gltfShader),
      mFramebuffer(framebuffer),
      mGltfModel(gltfModel),
      mRenderData(renderData),
      mUniformBuffer(UniformBuffer::make(0, 2 * sizeof(glm::mat4))),
      mShaderStorageBufferJointMatrices(
          ShaderStorageBuffer::make(1, gltfModel->getJointMatrices().size() * sizeof(glm::mat4))),
      mUserInterface(renderData)
{
}

void OGLRenderer::setSize(const int width, const int height)
{
    mRenderData.rdWidth  = width;
    mRenderData.rdHeight = height;

    mFramebuffer.resize(width, height);
    glViewport(0, 0, width, height);
}

OGLRenderer::~OGLRenderer() { mFramebuffer.cleanup(); }

void OGLRenderer::uploadData(const OGLMesh& vertexData)
{
    mRenderData.rdTriangleCount = vertexData.vertices.size();
    mVertexBuffer.uploadData(vertexData);
}

void OGLRenderer::draw()
{
    static float previousFrameStartTime = 0.0;

    float frameStartTime = glfwGetTime();

    mRenderData.rdFrameTime = frameStartTime - previousFrameStartTime;
    mRenderData.rdTickDiff  = frameStartTime - previousFrameStartTime;

    previousFrameStartTime = frameStartTime;
    mFramebuffer.bind();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);

    mProjectionMatrix = glm::perspective(
        glm::radians(90.0f), static_cast<float>(mRenderData.rdWidth) / static_cast<float>(mRenderData.rdHeight), 0.1f,
        100.0f);

    float t = glfwGetTime();

    glm::mat4 model = glm::mat4(1.0);

    mGltfShader->use();

    mViewMatrix = mCamera.getViewMatrix(mRenderData) * model;

    if(mUniformBuffer)
    {
        mUniformBuffer->uploadData(mViewMatrix, mProjectionMatrix);
    }

    if(mShaderStorageBufferJointMatrices)
    {
        mShaderStorageBufferJointMatrices->uploadData(mGltfModel->getJointMatrices());
    }

    mGltfModel->draw();

    mFramebuffer.bind();

    mFramebuffer.drawToScreen();

    mUIGenerateTimer.start();
    handleMovementKeys();
    mGltfModel->uploadVertexBuffers();
    mUserInterface.createFrame(mRenderData);
    mRenderData.rdUIGenerateTime = mUIGenerateTimer.stop();
    mUserInterface.render();

    mLastTickTime = glfwGetTime();
}

void OGLRenderer::handleKeyEvents(const int key, const int scancode, const int action, const int mods) {}

void OGLRenderer::handleMouseButtonEvents(const int button, const int action, const int mods)
{
    ImGuiIO& io = ImGui::GetIO();
    if(button >= 0 && button < ImGuiMouseButton_COUNT)
    {
        io.AddMouseButtonEvent(button, action == GLFW_PRESS);
    }

    if(io.WantCaptureMouse)
    {
        return;
    }

    if(button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if(action == GLFW_PRESS)
        {
            mMouseLock = true;
        }

        if(action == GLFW_RELEASE)
        {
            mMouseLock = false;
        }
    }

    if(mMouseLock)
    {
        if(glfwRawMouseMotionSupported())
        {
            glfwSetInputMode(mRenderData.rdWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        }
    }
}

void OGLRenderer::handleMousePositionEvents(double xPos, double yPos)
{
    ImGuiIO& io = ImGui::GetIO();

    io.AddMousePosEvent(static_cast<float>(xPos), static_cast<float>(yPos));

    if(io.WantCaptureMouse)
    {
        return;
    }

    const int mouseMoveRelX = static_cast<int>(xPos) - mMouseXPos;
    const int mouseMoveRelY = static_cast<int>(yPos) - mMouseYPos;

    if(mMouseLock)
    {
        mRenderData.rdViewAzimuth += mouseMoveRelX / 10.0f;

        if(mRenderData.rdViewAzimuth < 0.0f)
        {
            mRenderData.rdViewAzimuth += 360.0f;
        }

        if(mRenderData.rdViewAzimuth >= 360.0f)
        {
            mRenderData.rdViewAzimuth -= 360.0f;
        }

        mRenderData.rdViewElevation -= mouseMoveRelY / 10.0f;

        if(mRenderData.rdViewElevation > 89.0f)
        {
            mRenderData.rdViewElevation = 89.0f;
        }

        if(mRenderData.rdViewElevation < -89.0f)
        {
            mRenderData.rdViewElevation = -89.0f;
        }
    }

    mMouseXPos = static_cast<int>(xPos);
    mMouseYPos = static_cast<int>(yPos);
}

void OGLRenderer::handleMovementKeys()
{
    mRenderData.rdMoveForward = 0;
    if(glfwGetKey(mRenderData.rdWindow, GLFW_KEY_W) == GLFW_PRESS)
    {
        mRenderData.rdMoveForward += 1;
    }

    if(glfwGetKey(mRenderData.rdWindow, GLFW_KEY_S) == GLFW_PRESS)
    {
        mRenderData.rdMoveForward -= 1;
    }

    mRenderData.rdMoveRight = 0;
    if(glfwGetKey(mRenderData.rdWindow, GLFW_KEY_D) == GLFW_PRESS)
    {
        mRenderData.rdMoveRight += 1;
    }

    if(glfwGetKey(mRenderData.rdWindow, GLFW_KEY_A) == GLFW_PRESS)
    {
        mRenderData.rdMoveRight -= 1;
    }

    mRenderData.rdMoveUp = 0;
    if(glfwGetKey(mRenderData.rdWindow, GLFW_KEY_Q) == GLFW_PRESS)
    {
        mRenderData.rdMoveUp += 1;
    }

    if(glfwGetKey(mRenderData.rdWindow, GLFW_KEY_E) == GLFW_PRESS)
    {
        mRenderData.rdMoveUp -= 1;
    }
}
