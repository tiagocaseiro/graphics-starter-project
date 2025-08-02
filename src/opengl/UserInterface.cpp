#include "UserInterface.h"

#include <string>

#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "OGLRenderData.h"

void UserInterface::init(const OGLRenderData& renderData)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(renderData.rdWindow, true);
    static constexpr auto glslVersion = "#version 460 core";
    ImGui_ImplOpenGL3_Init(glslVersion);
}

void UserInterface::createFrame(const OGLRenderData& renderData)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuiWindowFlags imguiWindowFlags = 0;
    ImGui::SetNextWindowBgAlpha(0.8f);
    ImGui::Begin("Control", nullptr, imguiWindowFlags);

    static float newFps = 0.0f;
    if(renderData.rdFrameTime > 0.0f)
    {
        newFps = 1.0f / renderData.rdFrameTime;
    }

    framesPerSecond = averagingAlpha * framesPerSecond + (1 - averagingAlpha) * newFps;

    ImGui::Text("FPS:");
    ImGui::SameLine();
    ImGui::Text(std::to_string(framesPerSecond).c_str());
    ImGui::Separator();

    ImGui::Text("UI Generation Time:");
    ImGui::SameLine();
    ImGui::Text(std::to_string(renderData.rdUIGenerateTime).c_str());
    ImGui::SameLine();
    ImGui::Text("ms");

    ImGui::Text("Triangles:");
    ImGui::SameLine();
    ImGui::Text(std::to_string(renderData.rdTriangleCount).c_str());

    ImGui::Text("View Azimuth:");
    ImGui::SameLine();
    ImGui::Text("%s", std::to_string(renderData.rdViewAzimuth).c_str());

    ImGui::Text("View Elevation:");
    ImGui::SameLine();
    ImGui::Text("%s", std::to_string(renderData.rdViewElevation).c_str());

    std::string windowDims = std::to_string(renderData.rdWidth) + "x" + std::to_string(renderData.rdHeight);
    ImGui::Text("Window Dimensions:");
    ImGui::SameLine();
    ImGui::Text(windowDims.c_str());

    // ImGui::Text("Camera Position:");
    // ImGui::SameLine();
    // ImGui::Text("%s", std::to_string(renderData.rdCameraWorldPosition).c_str());

    ImGui::End();
}

void UserInterface::render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UserInterface::cleanup()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
