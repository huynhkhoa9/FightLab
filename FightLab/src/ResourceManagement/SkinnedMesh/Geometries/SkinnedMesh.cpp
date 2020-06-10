#include "SkinnedMesh.h"

void SkinnedMesh::drawNode()
{
}

SkinnedMesh::SkinnedMesh()
{
}

SkinnedMesh::~SkinnedMesh()
{
}

void SkinnedMesh::loadMaterials(tinygltf::Model& input)
{
	materials.resize(input.materials.size());
	for (size_t i = 0; i < input.materials.size(); i++) {
		// We only read the most basic properties required for our sample
		tinygltf::Material glTFMaterial = input.materials[i];
		// Get the base color factor
		if (glTFMaterial.values.find("baseColorFactor") != glTFMaterial.values.end()) {
			materials[i].baseColorFactor = glm::make_vec4(glTFMaterial.values["baseColorFactor"].ColorFactor().data());
		}
		// Get base color texture index
		if (glTFMaterial.values.find("baseColorTexture") != glTFMaterial.values.end()) {
			materials[i].baseColorTextureIndex = glTFMaterial.values["baseColorTexture"].TextureIndex();
		}
	}
}

void SkinnedMesh::loadNode(const tinygltf::Node& inputNode, const tinygltf::Model& input, Node* parent, uint32_t nodeIndex, std::vector<SkinnedVertex>& vertices, std::vector<uint32_t>& indices)
{
	Node* node = new Node{};
	node->parent = parent;
	node->matrix = glm::mat4(1.0f);
	node->index = nodeIndex;
	node->skin = inputNode.skin;

	// Get the local node matrix
		// It's either made up from translation, rotation, scale or a 4x4 matrix
	if (inputNode.translation.size() == 3) {
		node->matrix = glm::translate(node->matrix, glm::vec3(glm::make_vec3(inputNode.translation.data())));
	}
	if (inputNode.rotation.size() == 4) {
		glm::quat q = glm::make_quat(inputNode.rotation.data());
		node->matrix *= glm::mat4(q);
	}
	if (inputNode.scale.size() == 3) {
		node->matrix = glm::scale(node->matrix, glm::vec3(glm::make_vec3(inputNode.scale.data())));
	}
	if (inputNode.matrix.size() == 16) {
		node->matrix = glm::make_mat4x4(inputNode.matrix.data());
	};

	// Load node's children
	if (inputNode.children.size() > 0) {
		for (size_t i = 0; i < inputNode.children.size(); i++) {
			loadNode(input.nodes[inputNode.children[i]], input, node, inputNode.children[i], vertices, indices);
		}
	}

	// If the node contains mesh data, we load vertices and indices from the the buffers
		// In glTF this is done via accessors and buffer views
	if (inputNode.mesh > -1) {
		const tinygltf::Mesh mesh = input.meshes[inputNode.mesh];
		// Iterate through all primitives of this node's mesh
		for (size_t i = 0; i < mesh.primitives.size(); i++) {
			const tinygltf::Primitive& glTFPrimitive = mesh.primitives[i];
			uint32_t                   firstIndex = static_cast<uint32_t>(indices.size());
			uint32_t                   vertexStart = static_cast<uint32_t>(vertices.size());
			uint32_t                   indexCount = 0;
			bool                       hasSkin = false;
			// Vertices
			{
				const float* positionBuffer = nullptr;
				const float* normalsBuffer = nullptr;
				const float* texCoordsBuffer = nullptr;
				const uint16_t* jointIndicesBuffer = nullptr;
				const float* jointWeightsBuffer = nullptr;
				size_t          vertexCount = 0;

				// Get buffer data for vertex position
				if (glTFPrimitive.attributes.find("POSITION") != glTFPrimitive.attributes.end()) {
					const tinygltf::Accessor& accessor = input.accessors[glTFPrimitive.attributes.find("POSITION")->second];
					const tinygltf::BufferView& view = input.bufferViews[accessor.bufferView];
					positionBuffer = reinterpret_cast<const float*>(&(input.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
					vertexCount = accessor.count;
				}
				// Get buffer data for vertex normals
				if (glTFPrimitive.attributes.find("NORMAL") != glTFPrimitive.attributes.end()) {
					const tinygltf::Accessor& accessor = input.accessors[glTFPrimitive.attributes.find("NORMAL")->second];
					const tinygltf::BufferView& view = input.bufferViews[accessor.bufferView];
					normalsBuffer = reinterpret_cast<const float*>(&(input.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
				}
				// Get buffer data for vertex texture coordinates
				// glTF supports multiple sets, we only load the first one
				if (glTFPrimitive.attributes.find("TEXCOORD_0") != glTFPrimitive.attributes.end()) {
					const tinygltf::Accessor& accessor = input.accessors[glTFPrimitive.attributes.find("TEXCOORD_0")->second];
					const tinygltf::BufferView& view = input.bufferViews[accessor.bufferView];
					texCoordsBuffer = reinterpret_cast<const float*>(&(input.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
				}

				// POI: Get buffer data required for vertex skinning
				// Get vertex joint indices
				if (glTFPrimitive.attributes.find("JOINTS_0") != glTFPrimitive.attributes.end())
				{
					const tinygltf::Accessor& accessor = input.accessors[glTFPrimitive.attributes.find("JOINTS_0")->second];
					const tinygltf::BufferView& view = input.bufferViews[accessor.bufferView];
					jointIndicesBuffer = reinterpret_cast<const uint16_t*>(&(input.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
				}
				// Get vertex joint weights
				if (glTFPrimitive.attributes.find("WEIGHTS_0") != glTFPrimitive.attributes.end())
				{
					const tinygltf::Accessor& accessor = input.accessors[glTFPrimitive.attributes.find("WEIGHTS_0")->second];
					const tinygltf::BufferView& view = input.bufferViews[accessor.bufferView];
					jointWeightsBuffer = reinterpret_cast<const float*>(&(input.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
				}

				hasSkin = (jointIndicesBuffer && jointWeightsBuffer);

				// Append data to model's vertex buffer
				for (size_t v = 0; v < vertexCount; v++)
				{
					SkinnedVertex vert{};
					vert.pos = glm::vec4(glm::make_vec3(&positionBuffer[v * 3]), 1.0f);
					vert.normal = glm::normalize(glm::vec3(normalsBuffer ? glm::make_vec3(&normalsBuffer[v * 3]) : glm::vec3(0.0f)));
					vert.texCoord = texCoordsBuffer ? glm::make_vec2(&texCoordsBuffer[v * 2]) : glm::vec3(0.0f);
					vert.color = glm::vec3(1.0f);
					vert.jointIndices = hasSkin ? glm::vec4(glm::make_vec4(&jointIndicesBuffer[v * 4])) : glm::vec4(0.0f);
					vert.jointWeights = hasSkin ? glm::make_vec4(&jointWeightsBuffer[v * 4]) : glm::vec4(0.0f);
					vertices.push_back(vert);
				}
			}
			// Indices
			{
				const tinygltf::Accessor& accessor = input.accessors[glTFPrimitive.indices];
				const tinygltf::BufferView& bufferView = input.bufferViews[accessor.bufferView];
				const tinygltf::Buffer& buffer = input.buffers[bufferView.buffer];

				indexCount += static_cast<uint32_t>(accessor.count);

				// glTF supports different component types of indices
				switch (accessor.componentType) {
				case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT: {
					uint32_t* buf = new uint32_t[accessor.count];
					memcpy(buf, &buffer.data[accessor.byteOffset + bufferView.byteOffset], accessor.count * sizeof(uint32_t));
					for (size_t index = 0; index < accessor.count; index++) {
						indices.push_back(buf[index] + vertexStart);
					}
					break;
				}
				case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT: {
					uint16_t* buf = new uint16_t[accessor.count];
					memcpy(buf, &buffer.data[accessor.byteOffset + bufferView.byteOffset], accessor.count * sizeof(uint16_t));
					for (size_t index = 0; index < accessor.count; index++) {
						indices.push_back(buf[index] + vertexStart);
					}
					break;
				}
				case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE: {
					uint8_t* buf = new uint8_t[accessor.count];
					memcpy(buf, &buffer.data[accessor.byteOffset + bufferView.byteOffset], accessor.count * sizeof(uint8_t));
					for (size_t index = 0; index < accessor.count; index++) {
						indices.push_back(buf[index] + vertexStart);
					}
					break;
				}
				default:
					std::cerr << "Index component type " << accessor.componentType << " not supported!" << std::endl;
					return;
				}
			}
			Primitive primitive{};
			primitive.firstIndex = firstIndex;
			primitive.indexCount = indexCount;
			primitive.materialIndex = glTFPrimitive.material;
			node->mesh.primitives.push_back(primitive);
		}
	}

	if (parent) {
		parent->children.push_back(node);
	}
	else {
		nodes.push_back(node);
	}
}

void SkinnedMesh::loadSkin(tinygltf::Model& input)
{
	skins.resize(input.skins.size());

	for (size_t i = 0; i < input.skins.size(); i++)
	{
		tinygltf::Skin glTFSkin = input.skins[i];

		skins[i].name = glTFSkin.name;
		// Find the root node of the skeleton
		skins[i].skeletonRoot = nodeFromIndex(glTFSkin.skeleton);

		// Find joint nodes
		for (int jointIndex : glTFSkin.joints)
		{
			Node* node = nodeFromIndex(jointIndex);
			if (node)
			{
				skins[i].joints.push_back(node);
			}
		}

		// Get the inverse bind matrices from the buffer associated to this skin
		if (glTFSkin.inverseBindMatrices > -1)
		{
			const tinygltf::Accessor& accessor = input.accessors[glTFSkin.inverseBindMatrices];
			const tinygltf::BufferView& bufferView = input.bufferViews[accessor.bufferView];
			const tinygltf::Buffer& buffer = input.buffers[bufferView.buffer];
			skins[i].inverseBindMatrices.resize(accessor.count);
			memcpy(skins[i].inverseBindMatrices.data(), &buffer.data[accessor.byteOffset + bufferView.byteOffset], accessor.count * sizeof(glm::mat4));

			// Store inverse bind matrices for this skin in a shader storage buffer object
			// To keep this sample simple, we create a host visible shader storage buffer
			
		}
	}

}

void SkinnedMesh::Draw()
{
}

Node* SkinnedMesh::nodeFromIndex(uint32_t index)
{
	return nullptr;
}

Node* SkinnedMesh::findNode(Node* parent, uint32_t index)
{
	return nullptr;
}
