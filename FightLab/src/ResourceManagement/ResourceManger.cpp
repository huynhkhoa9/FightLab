#include "pch.h"
#include "ResourceManger.h"

VkShaderModule ResourceManger::LoadShader()
{
	return VkShaderModule();
}

VkImage ResourceManger::LoadTexture()
{
	return VkImage();
}

VkShaderModule ResourceManger::GetShader()
{
	return VkShaderModule();
}

VkSampler ResourceManger::GetImageSampler()
{
	return VkSampler();
}

void ResourceManger::CleanUp(const VkDevice& device)
{
}

void ResourceManger::CreateImage()
{
}

void ResourceManger::CreateImageView()
{
}

void ResourceManger::CreateSampler()
{
}
