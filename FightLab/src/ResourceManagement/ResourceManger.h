#pragma once
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
class ResourceManger
{
public:
	static std::map<std::string, VkShaderModule> ShaderModulesLibrary;
	static std::map<std::string, VkSampler> ImageSamplerLibrary;
	static std::map<std::string, VkImage> TextureImageLibrary;
	static std::map<std::string, VkImageView> TextureImageViewLibrary;

	//Load shader from file
	VkShaderModule LoadShader();

	//Load Texture from file
	VkImage LoadTexture();

	//Get shader
	VkShaderModule GetShader();

	//Get Sampler
	VkSampler GetImageSampler();

	//Clean Up resources
	static void CleanUp(const VkDevice& device);
private:
	ResourceManger() {}

	//Create an Image for the loaded texture
	void CreateImage();
	//Create an ImageView for the image
	void CreateImageView();
	//Create a Sampler for the ImageView
	void CreateSampler();
};
#endif
