#pragma once
#include "Joint.h"
#include "Utility.h"

class Skeleton
{
public:
	Skeleton();
	~Skeleton();

	std::vector<Joint> Joints;
	std::map <std::string, uint32_t> BoneIDMap;
	aiAnimation* CurrentAnimation;
	aiNode* Root;
	aiMatrix4x4 GlobalInverseTransform;
	void Update();
	
private:
	void readJointHeirarchy(const uint32_t& time, aiNode* pNode, const glm::mat4& ParentTransform)
	{
		std::string NodeName(pNode->mName.data);
		const aiNodeAnim* pNodeAnim = findNodeAnim(NodeName);
		glm::mat4 NodeTransformation = glm::mat4(1);
		if (pNodeAnim)
		{
			std::cout << "Found a Joint: " << NodeName << std::endl;

			glm::mat4 translation = (getTranslation(time, pNodeAnim));
			glm::mat4 rotation = (getRotation(time, pNodeAnim));
			glm::mat4 scaling = getScaling(time, pNodeAnim);

			NodeTransformation = translation * rotation /* scaling*/;
		}

		glm::mat4 GlobalTransfomation = ParentTransform * NodeTransformation;

		if (BoneIDMap.find(NodeName) != BoneIDMap.end()) {
			uint32_t BoneIndex = BoneIDMap[NodeName];
			Joints[BoneIndex].finalTransformation = ai2glmMat(GlobalInverseTransform) * (GlobalTransfomation) * ai2glmMat(Joints[BoneIndex].offset);
			
			std::cout << Joints[BoneIndex].finalTransformation[0][0] << " " << Joints[BoneIndex].finalTransformation[0][1] << " " << Joints[BoneIndex].finalTransformation[0][2] << " " << Joints[BoneIndex].finalTransformation[0][3] << std::endl
					  << Joints[BoneIndex].finalTransformation[1][0] << " " << Joints[BoneIndex].finalTransformation[1][1] << " " << Joints[BoneIndex].finalTransformation[1][2] << " " << Joints[BoneIndex].finalTransformation[1][3] << std::endl
				      << Joints[BoneIndex].finalTransformation[2][0] << " " << Joints[BoneIndex].finalTransformation[2][1] << " " << Joints[BoneIndex].finalTransformation[2][2] << " " << Joints[BoneIndex].finalTransformation[2][3] << std::endl
					  << Joints[BoneIndex].finalTransformation[3][0] << " " << Joints[BoneIndex].finalTransformation[3][1] << " " << Joints[BoneIndex].finalTransformation[3][2] << " " << Joints[BoneIndex].finalTransformation[3][3] << std::endl << std::endl;
		}
		for (uint32_t i = 0; i < pNode->mNumChildren; i++)
		{
			readJointHeirarchy(time, pNode->mChildren[i], GlobalTransfomation);
		}
	}

	aiNodeAnim* findNodeAnim(std::string NodeName)
	{
		for (uint32_t node = 0; node < CurrentAnimation->mNumChannels; node++)
		{
			if (CurrentAnimation->mChannels[node]->mNodeName.data == NodeName)
				return CurrentAnimation->mChannels[node];
		}

		return nullptr;
	}

	glm::mat4 getTranslation(uint32_t time, const aiNodeAnim* pNodeAnim);
	glm::mat4 getRotation(uint32_t time, const aiNodeAnim* pNodeAnim);
	glm::mat4 getScaling(uint32_t time, const aiNodeAnim* pNodeAnim);
	/* HELPER FUNCTIONS*/
	aiVector3D glm2aiVec(glm::vec3& vec)
	{
		return aiVector3D(vec.x, vec.y, vec.z);
	}
	aiMatrix4x4 glm2aiMat4(glm::mat4& mat)
	{
		aiMatrix4x4 aiMat;
		aiMat.a1 = mat[0][0];
		aiMat.a2 = mat[0][1];
		aiMat.a3 = mat[0][2];
		aiMat.a4 = mat[0][3];

		aiMat.b1 = mat[1][0];
		aiMat.b2 = mat[1][1];
		aiMat.b3 = mat[1][2];
		aiMat.b4 = mat[1][3];

		aiMat.c1 = mat[2][0];
		aiMat.c2 = mat[2][1];
		aiMat.c3 = mat[2][2];
		aiMat.c4 = mat[2][3];

		aiMat.d1 = mat[3][0];
		aiMat.d2 = mat[3][1];
		aiMat.d3 = mat[3][2];
		aiMat.d4 = mat[3][3];

		return aiMat;
	}
	glm::vec3 ai2glmVec(aiVector3D& vec)
	{
		return glm::vec3(vec.x, vec.y, vec.z);
	}
	glm::mat4 ai2glmMat(aiMatrix4x4& mat)
	{
		glm::mat4 aiMat;
		aiMat[0][0] = mat.a1;
		aiMat[0][1] = mat.a2;
		aiMat[0][2] = mat.a3;
		aiMat[0][3] = mat.a4;

		aiMat[1][0] = mat.b1;
		aiMat[1][1] = mat.b2;
		aiMat[1][2] = mat.b3;
		aiMat[1][3] = mat.b4;

		aiMat[2][0] = mat.c1;
		aiMat[2][1] = mat.c2;
		aiMat[2][2] = mat.c3;
		aiMat[2][3] = mat.c4;

		aiMat[3][0] = mat.d1;
		aiMat[3][1] = mat.d2;
		aiMat[3][2] = mat.d3;
		aiMat[3][3] = mat.d4;

		return aiMat;
	}
};