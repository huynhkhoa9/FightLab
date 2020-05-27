#pragma once
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
class ResourceManager
{
public:
	static std::map<const std::string&, VkShaderModule> ShaderModulesLibrary;
	static std::map<std::string, VkSampler> ImageSamplerLibrary;
	static std::map<std::string, VkImage> TextureImageLibrary;
	static std::map<std::string, VkImageView> TextureImageViewLibrary;

	//Load shader from file
	static VkShaderModule LoadShader(const std::string& filename, const VkDevice& logicalDevice);

	//Load Texture from file
	static VkImage LoadTexture(const std::string& filename, const std::string& textureName, const VkDevice& logicalDevice);

	//Get shader
	static VkShaderModule GetShader(const std::string& shaderName);

	//Get Sampler
	static VkSampler GetImageSampler(const std::string& textureName);

	//Clean Up resources
	static void CleanUp(const VkDevice& device);
private:
	ResourceManager() {}
	
	//Create an Image for the loaded texture
	void CreateImage();
	//Create an ImageView for the image
	void CreateImageView();
	//Create a Sampler for the ImageView
	void CreateSampler();
	//read file
	static std::vector<char> readFile(const std::string& filename) {
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			std::cout << "failed to open file!" << std::endl;
			throw std::runtime_error("failed to open file!");
		}
		else
		{
			std::cout << "We opened the file!" << std::endl;
			size_t fileSize = (size_t)file.tellg();
			std::vector<char> buffer(fileSize);
			file.seekg(0);
			file.read(buffer.data(), fileSize);
			file.close();

			return buffer;
		}
	}

};
#endif
