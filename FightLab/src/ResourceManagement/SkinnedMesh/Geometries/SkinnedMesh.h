#pragma once
#include "Vertex.h"
#include "ResourceManagement/Material/Material.h"
#include "ResourceManagement/SkinnedMesh/Animator/Animation.h"
#include "Utility.h"

class SkinnedMesh
{
public:
	std::vector<Mesh>      meshes;
	std::vector<Material>  materials;
	std::vector<Node*>     nodes;
	std::vector<Skin>      skins;
	std::vector<Animation> animations;
	void drawNode();

	SkinnedMesh();
	~SkinnedMesh();
	void loadMaterials(tinygltf::Model& input);
	
	void loadNode(const tinygltf::Node& inputNode, const tinygltf::Model& input, Node* parent, uint32_t nodeIndex ,std::vector<SkinnedVertex>& vertices, std::vector<uint32_t>& indices);
	void loadSkin(tinygltf::Model& input);
	void Draw();
	Node* nodeFromIndex(uint32_t index);
	Node* findNode(Node* parent, uint32_t index);
};