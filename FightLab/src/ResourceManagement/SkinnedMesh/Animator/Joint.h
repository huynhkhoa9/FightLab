#pragma once
#include "pch.h"

class Skeleton;

struct Joint
{
	Joint* pParent;
	Skeleton* pParentSkeleton;
	aiMatrix4x4 offset;
	glm::mat4 finalTransformation;
};