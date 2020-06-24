#pragma once
#include "Utility.h"
#include "Pose.h"

class Skeleton
{
protected:
	Pose mRestPose;
	Pose mBindPose;
	std::vector<glm::mat4> mInvBindPose;
	std::vector<std::string> mJointNames;

	void updateInverseBindPose();
public:
	Skeleton();
	Skeleton(const Pose& rest, const Pose& bind, const std::vector<std::string>& names);

	~Skeleton();

	void set(const Pose& rest, const Pose& bind, const std::vector<std::string>& names);

	Pose& getBindPose();
	Pose& getRestPose();
	std::vector<glm::mat4>& getInvBindPose();
	std::vector<std::string>& getJointNames();
	std::string& getJointName(unsigned int index);
};