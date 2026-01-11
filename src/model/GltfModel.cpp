#include "GltfModel.h"

#include <algorithm>

#include <tinygltf/tiny_gltf.h>

#include "GltfNode.h"
#include "Logger.h"

#include "opengl/OGLRenderData.h"

static const std::string_view ATTRIBUTES[] = {"POSITION", "NORMAL", "TEXCOORD_0", "JOINTS_0", "WEIGHTS_0"};

static void initializeFromBuffer(const tinygltf::Model& model, const int accessorIndex, auto& destination)
{
    const tinygltf::Accessor& accessor     = model.accessors[accessorIndex];
    const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
    const tinygltf::Buffer& buffer         = model.buffers[bufferView.buffer];

    // if(destination.empty())
    // {
    destination.resize(accessor.count);
    // }

    memcpy(destination.data(), buffer.data.data() + bufferView.byteOffset, bufferView.byteLength);
}

int getAttributeIndex(std::string_view attribute)
{
    return std::distance(std::begin(ATTRIBUTES), std::ranges::find(ATTRIBUTES, attribute));
}

bool GltfModel::loadModel(OGLRenderData& renderData, const std::string& modelFilename,
                          const std::string& textureFilename)
{
    if(mTex.loadTexture(textureFilename, false) == false)
    {
        return false;
    }

    mModel = std::make_shared<tinygltf::Model>();
    tinygltf::TinyGLTF gltfLoader;
    std::string loaderErrors;
    std::string loaderWarnings;
    bool result = false;

    result = gltfLoader.LoadASCIIFromFile(mModel.get(), &loaderErrors, &loaderWarnings, modelFilename);

    if(loaderWarnings.empty() == false)
    {
        Logger::log(1, "%s: warnings while loading glTF model:\n%s\n", __FUNCTION__, loaderWarnings.c_str());
    }

    if(loaderWarnings.empty() == false)
    {
        Logger::log(1, "%s: errors while loading glTF model:\n%s\n", __FUNCTION__, loaderErrors.c_str());
    }

    if(result == false)
    {
        Logger::log(1, "%s error: while loading file %s\n", __FUNCTION__, modelFilename.c_str());
        return false;
    }

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);
    createVertexBuffers();
    createIndexBuffer();
    glBindVertexArray(0);

    renderData.rdGltfTriangleCount = getTriangleCount();

    const std::vector<int>& nodes = mModel->scenes[0].nodes;
    const tinygltf::Skin& skin    = mModel->skins[0];

    // mInverseBindMatrices.resize(skin.joints.size());

    int inverseBindMatricesIndex = skin.inverseBindMatrices;
    if(inverseBindMatricesIndex != -1)
    {
        initializeFromBuffer(*mModel, inverseBindMatricesIndex, mInverseBindMatrices);
    }

    mJointMatrices.resize(mInverseBindMatrices.size());

    int rootNode = nodes.at(0);

    mNodeToJoint.resize(mModel->nodes.size());

    const std::vector<int>& joints = skin.joints;
    for(int i = 0; i != joints.size(); i++)
    {
        const int nodeIndex     = joints[i];
        mNodeToJoint[nodeIndex] = i;
    }

    mRootNode = GltfNode::createRoot(rootNode, *mModel, mNodeToJoint, mInverseBindMatrices, mJointMatrices);

    initializeFromBuffer(*mModel, getAttributeIndex("JOINTS_0"), mJoints);

    initializeFromBuffer(*mModel, getAttributeIndex("WEIGHTS_0"), mWeights);

    std::cout << *mRootNode << std::endl;

    return true;
}

void GltfModel::draw()
{
    const tinygltf::Primitive& primitives   = mModel->meshes.front().primitives.front();
    const tinygltf::Accessor& indexAccessor = mModel->accessors[primitives.indices];

    GLuint drawMode = GL_TRIANGLES;
    switch(primitives.mode)
    {
        case TINYGLTF_MODE_TRIANGLES:
            drawMode = GL_TRIANGLES;
            break;
        default:
            Logger::log(1, "%s error: unknown draw mode %i\n", __FUNCTION__, drawMode);
            break;
    }

    mTex.bind();
    glBindVertexArray(mVAO);
    glDrawElements(drawMode, indexAccessor.count, indexAccessor.componentType, nullptr);

    glBindVertexArray(0);
    mTex.unbind();
}

void GltfModel::cleanup()
{
    glDeleteBuffers(mVertexVBO.size(), mVertexVBO.data());
    glDeleteBuffers(1, &mVAO);
    glDeleteBuffers(1, &mIndexVBO);
    mTex.cleanup();
    mModel.reset();
}

void GltfModel::uploadVertexBuffers()
{
    // for(int i = 0; i < std::size(ATTRIBUTES); i++)
    for(int i = 0; i < 3; i++)
    {
        const tinygltf::Accessor& accessor     = mModel->accessors[i];
        const tinygltf::BufferView& bufferView = mModel->bufferViews[accessor.bufferView];
        const tinygltf::Buffer& buffer         = mModel->buffers[bufferView.buffer];

        glBindBuffer(GL_ARRAY_BUFFER, mVertexVBO[i]);
        glBufferData(GL_ARRAY_BUFFER, bufferView.byteLength, &buffer.data[0] + bufferView.byteOffset, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void GltfModel::uploadIndexBuffer()
{
    const tinygltf::Primitive& primitives  = mModel->meshes.front().primitives.front();
    const tinygltf::Accessor& accessor     = mModel->accessors[primitives.indices];
    const tinygltf::BufferView& bufferView = mModel->bufferViews[accessor.bufferView];
    const tinygltf::Buffer& buffer         = mModel->buffers[bufferView.buffer];

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferView.byteLength, &buffer.data[0] + bufferView.byteOffset,
                 GL_STATIC_DRAW);
}

void GltfModel::createVertexBuffers()
{
    const tinygltf::Primitive& primitives = mModel->meshes.front().primitives.front();
    mVertexVBO.resize(primitives.attributes.size());

    for(const auto& [attribType, accessorNum] : primitives.attributes)
    {
        const tinygltf::Accessor& accessor     = mModel->accessors[accessorNum];
        const tinygltf::BufferView& bufferView = mModel->bufferViews[accessor.bufferView];
        const tinygltf::Buffer& buffer         = mModel->buffers[bufferView.buffer];

        if(std::ranges::contains(ATTRIBUTES, attribType) == false)
        {
            continue;
        }

        int dataSize = 1;
        switch(accessor.type)
        {
            case TINYGLTF_TYPE_SCALAR:
                dataSize = 1;
                break;
            case TINYGLTF_TYPE_VEC2:
                dataSize = 2;
                break;
            case TINYGLTF_TYPE_VEC3:
                dataSize = 3;
                break;
            case TINYGLTF_TYPE_VEC4:
                dataSize = 4;
                break;
            default:
                Logger::log(1, "%s error: accessor %i uses data size %i\n", __FUNCTION__, accessorNum, dataSize);
                break;
        }

        GLuint dataType = GL_FLOAT;
        switch(accessor.componentType)
        {
            case TINYGLTF_COMPONENT_TYPE_FLOAT:
                dataType = GL_FLOAT;
                break;
            default:
                Logger::log(1, "%s error: accessor %i uses unknown data type %i\n", __FUNCTION__, accessorNum,
                            dataType);
                break;
        }

        const int attributeIndex = getAttributeIndex(attribType);

        glGenBuffers(1, &mVertexVBO[attributeIndex]);
        glBindBuffer(GL_ARRAY_BUFFER, mVertexVBO[attributeIndex]);

        glVertexAttribPointer(attributeIndex, dataSize, dataType, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(attributeIndex);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void GltfModel::createIndexBuffer()
{
    glGenBuffers(1, &mIndexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexVBO);
}

int GltfModel::getTriangleCount() const
{
    const tinygltf::Primitive& primitives = mModel->meshes.front().primitives.front();
    const tinygltf::Accessor& accessor    = mModel->accessors[primitives.indices];
    return accessor.count;
}
