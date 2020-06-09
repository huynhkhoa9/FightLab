#pragma once
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "ResourceManagement/SkinnedMesh/Geometries/SkinnedMesh.h"
#include "Utility.h"
struct VkContext
{
	VkInstance instance;
	VkSurfaceKHR surface;

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;

	bool framebufferResized = false;

	GLFWwindow* window;
};

struct ModelContext
{
	uint32_t indexOffset = 0;
	uint32_t vertexCount = 0;
};

class ResourceManager
{
public:
	std::map<const std::string, uint32_t> ShaderModulesIDMap;
	std::map<const std::string, uint32_t> ImageSamplerIDMap;
	std::map<const std::string, uint32_t> TextureImageIDMap;
	std::map<const std::string, uint32_t> TextureImageViewIDMap;
	std::map<const std::string, uint32_t> ModelIDMap;
	std::map<const std::string, uint32_t> MeshIDMap;

	//Shader
	std::vector<VkShaderModule> ShaderModulesLibrary;

	//Texture, Texure ImageView, and Sampler
	std::vector<VkImage> TextureImageLibrary;
	std::vector<VkImageView> TextureImageViewLibrary;
	std::vector<VkSampler> ImageSamplerLibrary;
	std::vector<SkinnedMesh> meshes;
	//SkinnedMesh and Animation


	ResourceManager(const VkContext& cntxt, const VmaAllocator& allocator, const VkCommandPool& cmdPool, const VkQueue& gfxQueue) {
		context = cntxt;
		memAllocator = allocator;
		graphicsQueue = gfxQueue;
		commandPool = cmdPool;
	}
	ResourceManager(const ResourceManager& r)
	{
		context = r.context;
		memAllocator = r.memAllocator;
		graphicsQueue = r.graphicsQueue;
		commandPool = r.commandPool;
	}
	ResourceManager() 
	{
	}
	~ResourceManager() {}

	//Load shader from file
	VkShaderModule LoadShader(const std::string& filename, const std::string& shaderName);

	//Load Texture from file
	void LoadTexture(const std::string& filename, const std::string& textureName);

	//Load Skinned Mesh
	bool LoadSkinnedMesh(const std::string& path, const std::string& skinnedMeshName);

	//Get shader
	VkShaderModule GetShader(const std::string& shaderName);

	//Get Sampler
	VkSampler GetImageSampler(const std::string& textureName);

	//Get ImageView
	VkImageView GetImageView(const std::string& imageViewName);

	//Clean Up resources
	void CleanUp();
private:
	VkContext context;
	VkCommandPool commandPool;
	VkQueue graphicsQueue;
	VmaAllocator memAllocator;
	std::string skinnedMeshDirectory;
	std::vector<VmaAllocation> textureAllocations;

	//Create an Image for the loaded texture
	void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags properties,
		VkImage& image, VmaAllocation& allocation);

	//Create a vkImage from file
	VkImage createTexture(const std::string& fileName, const std::string& textureName, uint32_t& mipLevels);

	//Create an ImageView for the image
	void CreateImageView(const std::string name, VkImage image, VkFormat format, VkImageAspectFlags aspect, uint32_t mipLevels);

	//Create a Sampler for the ImageView
	void CreateSampler(const std::string name, int mipLevels);

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

	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags requiredProperties,
		VkBuffer& buffer, VmaAllocation& allocation) {
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VmaAllocationCreateInfo allocInfo = {};
		allocInfo.requiredFlags = requiredProperties;
		//allocInfo.preferredFlags = preferredProperties;
		//allocInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;

		vmaCreateBuffer(memAllocator, &bufferInfo, &allocInfo, &buffer, &allocation, nullptr);
	}

	VkCommandBuffer beginSingleTimeCommands() {
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = commandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(context.device, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	void endSingleTimeCommands(VkCommandBuffer commandBuffer) {
		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(graphicsQueue);

		vkFreeCommandBuffers(context.device, commandPool, 1, &commandBuffer);
	}

	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
		VkCommandBuffer commandBuffer = beginSingleTimeCommands();

		VkBufferCopy copyRegion{};
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		endSingleTimeCommands(commandBuffer);
	}

	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels) {
		VkCommandBuffer commandBuffer = beginSingleTimeCommands();

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = mipLevels;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}
		else {
			throw std::invalid_argument("unsupported layout transition!");
		}

		vkCmdPipelineBarrier(
			commandBuffer,
			sourceStage, destinationStage,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		endSingleTimeCommands(commandBuffer);
	}

	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
		VkCommandBuffer commandBuffer = beginSingleTimeCommands();

		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = {
			width,
			height,
			1
		};

		vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		endSingleTimeCommands(commandBuffer);
	}
	
	void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
	
	void processNode(aiNode* node, const aiScene* scene, const std::string& skinnedMeshName);
	SkinnedMesh processMesh(aiMesh* mesh, const aiScene* scene, const std::string& skinnedMeshName);
};
#endif
