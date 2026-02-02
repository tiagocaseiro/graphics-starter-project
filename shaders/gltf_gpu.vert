#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec4 aJointNum;
layout(location = 4) in vec4 aJointWeight;

layout(location = 0) out vec3 normal;
layout(location = 1) out vec2 texCoord;

layout(std140, binding = 0) uniform Matrices
{
    mat4 view;
    mat4 projection;
};

layout(std430, binding = 1) readonly buffer JointMatrices { mat4 jointMat[]; };

// layout(std430, binding = 2) readonly buffer JointDualQuats { mat2x4 jointDQs[]; };

// void getJointTransform(ivec4 joints, vec4 weights)
// {
//     mat2x4 dq0 = jointDQs[joints.x];
//     mat2x4 dq1 = jointDQs[joints.y];
//     mat2x4 dq2 = jointDQs[joints.z];
//     mat2x4 dq3 = jointDQs[joints.w];

//     weights.y *= sign(dot(dq0[0], dq1[0]));
//     weights.z *= sign(dot(dq0[0], dq2[0]));
//     weights.w *= sign(dot(dq0[0], dq3[0]));

//     mat2x4 result = weights.x * dq0 + weights.y * dq1 + weights.z * dq2 + weights.w * dq3;

//     float norm = length(result[0]);

//     return result / norm;
// }

void main()
{
    normal   = aNormal;
    texCoord = aTexCoord;

    mat4 skinMat = aJointWeight.x * jointMat[int(aJointNum.x)] + aJointWeight.y * jointMat[int(aJointNum.y)] +
                   aJointWeight.z * jointMat[int(aJointNum.z)] + aJointWeight.w * jointMat[int(aJointNum.w)];

    gl_Position = projection * view * skinMat * vec4(aPos, 1.0);
}