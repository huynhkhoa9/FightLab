#include "pch.h"
#include "ResourceManger.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

VkShaderModule ResourceManager::LoadShader(const std::string& filename, const VkDevice& logicalDevice)
{
	auto shaderFile = readFile(filename);
	VkShaderModule shaderModule;
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = shaderFile.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderFile.data());
	
	if (vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");
	}

	return shaderModule;
}

VkImage ResourceManager::LoadTexture(const std::string& filename, const std::string& textureName, const VkDevice& logicalDevice)
{
	return VkImage();
}

VkShaderModule ResourceManager::GetShader(const std::string& shaderName)
{
	return VkShaderModule();
}

VkSampler ResourceManager::GetImageSampler(const std::string& textureName)
{
	return VkSampler();
}

void ResourceManager::CleanUp(const VkDevice& device)
{
}

void ResourceManager::CreateImage()
{
}

void ResourceManager::CreateImageView()
{
}

void ResourceManager::CreateSampler()
{
}
