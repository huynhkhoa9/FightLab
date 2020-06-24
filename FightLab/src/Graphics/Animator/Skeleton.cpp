#include "pch.h"
#include "Skeleton.h"

Skeleton::Skeleton()
{
}

Skeleton::Skeleton(const Pose& rest, const Pose& bind, const std::vector<std::string>& names)
{
	set(rest, bind, names);
}

Skeleton::~Skeleton()
{
}

void Skeleton::updateInverseBindPose()
{
	unsigned int size = mBindPose.size();
	mInvBindPose.resize(size);

	for (unsigned int i = 0; i < size; ++i) {
		Transform world = mBindPose.getGlobalTransform(i);
		mInvBindPose[i] = glm::inverse(transform2Matrix(world));
	}
}

void Skeleton::set(const Pose& rest, const Pose& bind, const std::vector<std::string>& names)
{
	mRestPose = rest;
	mBindPose = bind;
	mJointNames = names;
	updateInverseBindPose();
}

Pose& Skeleton::getBindPose()
{
	return mBindPose;
}

Pose& Skeleton::getRestPose()
{
	return mRestPose;
}

std::vector<glm::mat4>& Skeleton::getInvBindPose()
{
	return mInvBindPose;
}

std::vector<std::string>& Skeleton::getJointNames()
{
	return mJointNames;
}

std::string& Skeleton::getJointName(unsigned int index)
{
	return mJointNames[index];
}
