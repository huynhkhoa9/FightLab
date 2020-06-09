#include "pch.h"
#include "ResourceManger.h"
#include "Graphics/RenderBackEnd/RenderManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

VkShaderModule ResourceManager::LoadShader(const std::string& filename, const std::string& shaderName)
{
	auto shaderFile = readFile(filename);
	VkShaderModule shaderModule;
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = shaderFile.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderFile.data());
	
	if (vkCreateShaderModule(context.device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");
	}

	ShaderModulesIDMap[shaderName] = static_cast<unsigned int>(ShaderModulesLibrary.size());
	ShaderModulesLibrary.push_back(shaderModule);
	return shaderModule;
}

void ResourceManager::LoadTexture(const std::string& filename, const std::string& textureName)
{
	uint32_t mip = 0;
	createTexture(filename, textureName, mip);
	CreateImageView(textureName, TextureImageLibrary[TextureImageIDMap[textureName]], VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, mip);
	CreateSampler(textureName, mip);
}

bool ResourceManager::LoadSkinnedMesh(const std::string& path, const std::string& skinnedMeshName)
{
	// read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return false;
	}
	// retrieve the directory path of the filepath
	skinnedMeshDirectory = path.substr(0, path.find_last_of('/'));

	// process ASSIMP's root node recursively
	processNode(scene->mRootNode, scene, skinnedMeshName);
	return true;
}

VkShaderModule ResourceManager::GetShader(const std::string& shaderName)
{
	return ShaderModulesLibrary[ShaderModulesIDMap[shaderName]];
}

VkSampler ResourceManager::GetImageSampler(const std::string& textureName)
{
	return ImageSamplerLibrary[ImageSamplerIDMap[textureName]];
}

VkImageView ResourceManager::GetImageView(const std::string& imageViewName)
{
	return TextureImageViewLibrary[TextureImageIDMap[imageViewName]];
}

void ResourceManager::CleanUp()
{
	if (ShaderModulesLibrary.size() > 0)
	{
		for (int i = 0; i < ShaderModulesLibrary.size(); i++)
		{
			vkDestroyShaderModule(context.device, ShaderModulesLibrary[i], nullptr);
		}
	}

	if (ImageSamplerLibrary.size() > 0)
	{
		for (int i = 0; i < ImageSamplerLibrary.size(); i++)
		{
			vkDestroySampler(context.device, ImageSamplerLibrary[i], nullptr);
			vkDestroyImageView(context.device, TextureImageViewLibrary[i], nullptr);
			vmaDestroyImage(memAllocator, TextureImageLibrary[i], textureAllocations[i]);
		}
	}
}

void ResourceManager::createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VmaAllocation& allocation)
{
	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = static_cast<uint32_t>(width);
	imageInfo.extent.height = static_cast<uint32_t>(height);
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = mipLevels;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.flags = 0; // Optional

	VmaAllocationCreateInfo allocInfo = {};
	allocInfo.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

	vmaCreateImage(memAllocator, &imageInfo, &allocInfo, &image, &allocation, nullptr);
}

VkImage ResourceManager::createTexture(const std::string& fileName, const std::string& textureName, uint32_t& mipLevels)
{
	int texWidth, texHeight, texChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	stbi_uc* pixels = stbi_load(fileName.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	VkDeviceSize imageSize = texWidth * texHeight * 4;

	mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;

	if (!pixels) {
		throw std::runtime_error("failed to load texture image!");
	}

	VkBuffer stagingBuffer;
	VmaAllocation staging;

	createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer, staging);

	void* mappedData;
	vmaMapMemory(memAllocator, staging, &mappedData);
	memcpy(mappedData, pixels, static_cast<size_t>(imageSize));
	vmaUnmapMemory(memAllocator, staging);

	stbi_image_free(pixels);
	VkImage textureImage;
	VmaAllocation txtAllocation;
	createImage(texWidth, texHeight, mipLevels, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, txtAllocation);

	transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mipLevels);
	copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));

	generateMipmaps(textureImage, VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight, mipLevels);
	TextureImageIDMap.insert(std::make_pair(textureName, (uint32_t)TextureImageLibrary.size()));
	TextureImageLibrary.push_back(textureImage);
	textureAllocations.push_back(txtAllocation);

	vmaDestroyBuffer(memAllocator, stagingBuffer, staging);
	return textureImage;
}

void ResourceManager::CreateImageView(const std::string name, VkImage image, VkFormat format, VkImageAspectFlags aspect, uint32_t mipLevels)
{
	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = aspect;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = mipLevels;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VkImageView imageView;
	if (vkCreateImageView(context.device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture image view!");
	}

	TextureImageViewIDMap[name] = (uint32_t)TextureImageViewLibrary.size();
	TextureImageViewLibrary.push_back(imageView);
}

void ResourceManager::CreateSampler(const std::string name, int mipLevels)
{
	VkSampler textureSampler;

	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = VK_TRUE;
	//samplerInfo.anisotropyEnable = VK_FALSE;
	//samplerInfo.maxAnisotropy = 1.0f;
	samplerInfo.maxAnisotropy = 16.0f;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.minLod = 0.0f; // Optional
	samplerInfo.maxLod = static_cast<float>(mipLevels);
	samplerInfo.mipLodBias = 0.0f; // Optional

	if (vkCreateSampler(context.device, &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture sampler!");
	}

	ImageSamplerIDMap[name] = (uint32_t)ImageSamplerLibrary.size();
	ImageSamplerLibrary.push_back(textureSampler);
}

void ResourceManager::generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels)
{
	// Check if image format supports linear blitting
	VkFormatProperties formatProperties;
	vkGetPhysicalDeviceFormatProperties(context.physicalDevice, imageFormat, &formatProperties);

	if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
		throw std::runtime_error("texture image format does not support linear blitting!");
	}

	VkCommandBuffer commandBuffer = beginSingleTimeCommands();

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.image = image;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.subresourceRange.levelCount = 1;

	int32_t mipWidth = texWidth;
	int32_t mipHeight = texHeight;

	for (uint32_t i = 1; i < mipLevels; i++) {
		barrier.subresourceRange.baseMipLevel = i - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier);

		VkImageBlit blit{};
		blit.srcOffsets[0] = { 0, 0, 0 };
		blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
		blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.srcSubresource.mipLevel = i - 1;
		blit.srcSubresource.baseArrayLayer = 0;
		blit.srcSubresource.layerCount = 1;
		blit.dstOffsets[0] = { 0, 0, 0 };
		blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
		blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.dstSubresource.mipLevel = i;
		blit.dstSubresource.baseArrayLayer = 0;
		blit.dstSubresource.layerCount = 1;

		vkCmdBlitImage(commandBuffer,
			image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1, &blit,
			VK_FILTER_LINEAR);

		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier);

		if (mipWidth > 1) mipWidth /= 2;
		if (mipHeight > 1) mipHeight /= 2;
	}

	barrier.subresourceRange.baseMipLevel = mipLevels - 1;
	barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	vkCmdPipelineBarrier(commandBuffer,
		VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
		0, nullptr,
		0, nullptr,
		1, &barrier);

	endSingleTimeCommands(commandBuffer);
}

// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
void ResourceManager::processNode(aiNode* node, const aiScene* scene, const std::string& skinnedMeshName)
{
	// process each mesh located at the current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		MeshIDMap[skinnedMeshName] = static_cast<uint32_t>(meshes.size());
		meshes.push_back(processMesh(mesh, scene, skinnedMeshName));
		std::cout << skinnedMeshDirectory + "/viking-room.png" << std::endl;

		LoadTexture(skinnedMeshDirectory + "/viking_room.png", skinnedMeshName);
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene, skinnedMeshName);
	}

}

SkinnedMesh ResourceManager::processMesh(aiMesh* mesh, const aiScene* scene, const std::string& skinnedMeshName)
{
	// data to fill
	SkinnedMesh skinnedMesh;
	//fill out the mesh's skeleton
	skinnedMesh.JointDataPV.resize(mesh->mNumVertices);
	skinnedMesh.skeleton.Joints.resize(mesh->mNumBones);
	skinnedMesh.skeleton.Root = scene->mRootNode;
	skinnedMesh.skeleton.GlobalInverseTransform = scene->mRootNode->mTransformation.Inverse();
	skinnedMesh.skeleton.CurrentAnimation = scene->mAnimations[0];
	std::cout << skinnedMesh.skeleton.CurrentAnimation->mName.data << std::endl;
	for (uint32_t b = 0; b < mesh->mNumBones; b++)
	{
		aiBone* bone = mesh->mBones[b];
		skinnedMesh.skeleton.BoneIDMap[std::string(bone->mName.data)] = b;
		skinnedMesh.skeleton.Joints[b].offset = mesh->mBones[b]->mOffsetMatrix;
		for (uint32_t id = 0; id < bone->mNumWeights; id++)
		{
			uint32_t vertexID = bone->mWeights[id].mVertexId;
			float weight = bone->mWeights[id].mWeight;
			skinnedMesh.JointDataPV[vertexID].add(b, weight);
		}
	}

	// walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		SkinnedVertex vertex;
		glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.pos = vector;
		// normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;
		// texture coordinates
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoord = vec;
		}
		else
			vertex.texCoord = glm::vec2(0.0f, 0.0f);
		/*// tangent
		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.Tangent = vector;
		// bitangent
		vector.x = mesh->mBitangents[i].x;
		vector.y = mesh->mBitangents[i].y;
		vector.z = mesh->mBitangents[i].z;
		vertex.Bitangent = vector;*/
		for (uint8_t e = 0; e < MAX_JOINT_PER_VERTEX; e++)
		{
			vertex.boneIDs[e] = skinnedMesh.JointDataPV[i].IDs[e];
			vertex.boneWeights[e] = skinnedMesh.JointDataPV[i].weights[e];
		}
		
		skinnedMesh.vertices.push_back(vertex);
	}
	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			skinnedMesh.indices.push_back(face.mIndices[j]);
	}

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	skinnedMesh.skeleton.Update();
	
	// return a mesh object created from the extracted mesh data
	return skinnedMesh;
}





