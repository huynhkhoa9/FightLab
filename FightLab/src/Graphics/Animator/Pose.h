#pragma once
#include "Core/Transform.h"

class Pose
{
private:
	std::vector<Transform> mJoints;
	std::vector<int> mParents;
public:
	Pose();
	Pose(const Pose& p);
	Pose& operator=(const Pose& p);
	Pose(unsigned int numJoints);
	
	void resize(unsigned int size);
	unsigned int size();

	int getParent(unsigned int index);
	void setParent(unsigned int index, int parent);

	Transform getLocalTransform(unsigned int index);
	void setLocalTransform(unsigned int index, const Transform& transform);

	Transform getGlobalTransform(unsigned int index);
	Transform operator[](unsigned int index);

	void getMatrixPalette(std::vector<glm::mat4>& out);
	bool operator==(const Pose& other);
	bool operator!=(const Pose& other);
};

