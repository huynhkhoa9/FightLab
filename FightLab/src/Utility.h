#pragma once
#include "pch.h"
#include "Graphics/RenderBackEnd/VKBuffer.hpp"

struct Primitive {
	uint32_t firstIndex;
	uint32_t indexCount;
	int32_t materialIndex;
};

struct Mesh {
	std::vector<Primitive> primitives;
};

struct Node
{
	Node* parent;
	std::string         name;
	uint32_t            index;
	std::vector<Node*> children;
	Mesh                mesh;
	glm::vec3           translation{};
	glm::vec3           scale{ 1.0f };
	glm::quat           rotation{};
	int32_t             skin = -1;
	glm::mat4           matrix;
	glm::mat4 getLocalMatrix()
	{
		return glm::translate(glm::mat4(1.0f), translation) * glm::mat4(rotation) * glm::scale(glm::mat4(1.0f), scale) * matrix;
	}
};

struct Skin
{
	std::string            name;
	Node* skeletonRoot =   nullptr;
	std::vector<glm::mat4> inverseBindMatrices;
	std::vector<Node*>     joints;
	Buffer				   ssbo;
	VkDescriptorSet        descriptorSet;
};

