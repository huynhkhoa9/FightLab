#pragma once
#include "pch.h"

struct SkinnedVertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec3 normal;
    glm::vec2 texCoord;
    glm::vec4 jointWeights;
    glm::ivec4 jointIndices;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(SkinnedVertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 6> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 6> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(SkinnedVertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(SkinnedVertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(SkinnedVertex, color);

        attributeDescriptions[3].binding = 0;
        attributeDescriptions[3].location = 3;
        attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[3].offset = offsetof(SkinnedVertex, texCoord);

        attributeDescriptions[4].binding = 0;
        attributeDescriptions[4].location = 4;
        attributeDescriptions[4].format = VK_FORMAT_R32G32B32A32_SFLOAT;
        attributeDescriptions[4].offset = offsetof(SkinnedVertex, jointWeights);

        attributeDescriptions[5].binding = 0;
        attributeDescriptions[5].location = 5;
        attributeDescriptions[5].format = VK_FORMAT_R32G32B32A32_SINT;
        attributeDescriptions[5].offset = offsetof(SkinnedVertex, jointIndices);

        return attributeDescriptions;
    }

    bool operator==(const SkinnedVertex& other) const {
        return pos == other.pos && color == other.color && texCoord == other.texCoord;
    }
};

namespace std {
    template<> struct hash<SkinnedVertex> {
        size_t operator()(SkinnedVertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^
                (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
                (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}

struct VertexJointData
{
    std::array<uint32_t, MAX_JOINT_PER_VERTEX> IDs;
    std::array<float, MAX_JOINT_PER_VERTEX> weights;

    // Ad bone weighting to vertex info
    void add(uint32_t jointID, float weight)
    {
        for (uint32_t i = 0; i < MAX_JOINT_PER_VERTEX; i++)
        {
            if (weights[i] == 0.0f)
            {
                IDs[i] = jointID;
                weights[i] = weight;
                return;
            }
        }
    }
};

struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
    glm::mat4 joints[MAX_JOINTS];
};