#pragma once

#include "Utility.h"
#include "SkinnedVertex.h"
#include "Graphics/RenderBackEnd/VulkanDevice.h"
#include "Graphics/Material/Material.h"
#include "Graphics/Animator/Animation.h"

class SkinnedMesh
{
public:
	struct Vertices
	{
		VkBuffer buffer;
		VmaAllocation alloc;
	}vertices;

	struct Indices
	{
		VkBuffer buffer;
		VmaAllocation alloc;
		uint32_t count;
	}indices;

	std::vector<Node*>		   nodes;
	std::vector<Skin>		   skins;
	std::vector<Mesh>          meshes;
	
	std::vector<Material>      materials;
	std::vector<VkImage>       textures;
	std::vector<VkSampler>     textureSamplers;

	std::vector<Animation>     animations;

	uint32_t activeAnimation = 0;

	VulkanDevice*              vulkanDevice;

	VkDescriptorSet descriptorSet;
	
	SkinnedMesh();
	~SkinnedMesh();
	
	void loadMaterials(tinygltf::Model& input);
	void loadNode(const tinygltf::Node& inputNode, const tinygltf::Model& input, Node* parent, uint32_t nodeIndex ,std::vector<SkinnedVertex>& vertices, std::vector<uint32_t>& indices);
	void loadSkin(tinygltf::Model& input);
	void loadAnimations(tinygltf::Model& input);
	void updateJoints(Node* node);
	void updateAnimation(float deltaTime);
	void Draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout);
	
private: 
	Node* nodeFromIndex(uint32_t index);
	Node* findNode(Node* parent, uint32_t index);
	
	glm::mat4 getNodeMatrix(Node* node);
	void drawNode(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, Node node);
};