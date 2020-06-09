#pragma once
#include <glm.hpp>
#include <vector>
#include <map>
#include <iterator>

#include "Vertex.h"
#include "ResourceManagement/Material/Material.h"
#include "ResourceManagement/SkinnedMesh/Animator/Skeleton.h"

struct SkinnedMesh
{
	std::vector<SkinnedVertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<VertexJointData> JointDataPV;
	Skeleton skeleton;
	aiAnimation* pAnimation;
	uint32_t vertexOffset;
	uint32_t indexOffset;
	float ticksPerSecond = 60;

	Material material;
};