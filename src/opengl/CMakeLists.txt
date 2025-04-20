cmake_minimum_required(VERSION 3.31)

add_library(opengl 
    OGLRenderer.cpp 
    OGLRenderData.cpp
    FrameBuffer.cpp
    VertexBuffer.cpp
)


target_include_directories(opengl PUBLIC ..)
