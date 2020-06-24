#pragma once
#include "pch.h"

struct Texture {
	VkSampler sampler;
	VkImage image;
	VkImageLayout imageLayout;
	VkDeviceMemory deviceMemory;
	VkImageView view;
	uint32_t width, height;
	uint32_t mipLevels;
};

struct Material
{
	glm::vec4 baseColorFactor = glm::vec4(1.0f);
	uint32_t baseColorTextureIndex;
	VkDescriptorSet descriptorSet;
};