#include "pch.h"
#include "Skeleton.h"

Skeleton::Skeleton()
{
}

Skeleton::~Skeleton()
{
}

void Skeleton::Update()
{
	glm::mat4 identity = glm::mat4(1);
	readJointHeirarchy(0,Root, identity);
}

glm::mat4 Skeleton::getTranslation(uint32_t time, const aiNodeAnim* pNodeAnim)
{
	for (uint32_t t = 0; t < pNodeAnim->mNumPositionKeys; t++)
	{
		if (time >= pNodeAnim->mPositionKeys[t].mTime)
		{
			glm::vec3 translationVec = ai2glmVec(pNodeAnim->mPositionKeys[t].mValue);
			glm::mat4 transformation = (glm::translate(glm::mat4(1), translationVec));
			//std::cout << translationVec.x << " " << translationVec.y << " " << translationVec.z << std::endl;

			/*std::cout << transformation[0][0] << " " << transformation[0][1] << " " << transformation[0][2] << " " << transformation[0][3] << std::endl
					  << transformation[1][0] << " " << transformation[1][1] << " " << transformation[1][2] << " " << transformation[1][3] << std::endl
					  << transformation[2][0] << " " << transformation[2][1] << " " << transformation[2][2] << " " << transformation[2][3] << std::endl
					  << transformation[3][0] << " " << transformation[3][1] << " " << transformation[3][2] << " " << transformation[3][3] << std::endl << std::endl;
			*/
			return transformation;
		}
	}
	return glm::mat4(1);
}

glm::mat4 Skeleton::getRotation(uint32_t time, const aiNodeAnim* pNodeAnim)
{
	for (uint32_t t = 0; t < pNodeAnim->mNumRotationKeys; t++)
	{
		if (time >= pNodeAnim->mRotationKeys[t].mTime)
		{
			glm::quat rotation = glm::quat(pNodeAnim->mRotationKeys[t].mValue.w,
											pNodeAnim->mRotationKeys[t].mValue.x, 
											pNodeAnim->mRotationKeys[t].mValue.y, 
											pNodeAnim->mRotationKeys[t].mValue.z);

			//std::cout << rotation.w << " " << rotation.x << " " << rotation.y << " " << rotation.z << std::endl;

			return (glm::toMat4(rotation));
		}
	}
	return glm::mat4(1);
}

glm::mat4 Skeleton::getScaling(uint32_t time, const aiNodeAnim* pNodeAnim)
{
	for (uint32_t t = 0; t < pNodeAnim->mNumScalingKeys; t++)
	{
		if (time >= pNodeAnim->mScalingKeys[t].mTime)
		{
			glm::vec3 scalingVec = ai2glmVec(pNodeAnim->mScalingKeys[t].mValue);
			return glm::scale(scalingVec);
		}
	}
	return glm::mat4(1);
}

