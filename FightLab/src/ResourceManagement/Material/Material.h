#pragma once
#include "pch.h"

struct Material
{
	glm::vec4 baseColorFactor = glm::vec4(1.0f);
	uint32_t baseColorTextureIndex;
	VkDescriptorSet descriptorSet;
};