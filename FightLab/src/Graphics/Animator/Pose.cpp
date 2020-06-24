#include "pch.h"
#include "Pose.h"

Pose::Pose()
{
}

Pose::Pose(const Pose& p)
{
	*this = p;
}

Pose& Pose::operator=(const Pose& p)
{
	if (&p == this)
	{ 
		return *this;
	} 
	if (mParents.size() != p.mParents.size())
	{ 
		mParents.resize(p.mParents.size());
	}
	if (mJoints.size() != p.mJoints.size())
	{
		mJoints.resize(p.mJoints.size());
	}
	if (mParents.size() != 0)
	{
		memcpy(&mParents[0], &p.mParents[0], sizeof(int) * mParents.size());
	}
	if (mJoints.size() != 0)
	{ 
		memcpy(&mJoints[0], &p.mJoints[0], sizeof(Transform) * mJoints.size()); 
	}
	
	return *this;

}

Pose::Pose(unsigned int numJoints)
{
	resize(numJoints);
}

void Pose::resize(unsigned int size)
{
	mParents.resize(size);
	mJoints.resize(size);
}

unsigned int Pose::size()
{
	return mJoints.size();
}

int Pose::getParent(unsigned int index)
{
	return mParents[index];
}

void Pose::setParent(unsigned int index, int parent)
{
	mParents[index] = parent;
}

Transform Pose::getLocalTransform(unsigned int index)
{
	return mJoints[index];
}

void Pose::setLocalTransform(unsigned int index, const Transform& transform)
{
	mJoints[index] = transform;
}

Transform Pose::getGlobalTransform(unsigned int index)
{
	Transform result = mJoints[index];
	for (int p = mParents[index]; p >= 0; p = mParents[p])
	{
		result = combine(mJoints[p], result);
	}

	return result;
}

Transform Pose::operator[](unsigned int index)
{
	return getGlobalTransform(index);
}

void Pose::getMatrixPalette(std::vector<glm::mat4>& out)
{
	unsigned int size = this->size();
	if (out.size() != size)
	{
		out.resize(size);
	}
	for (unsigned int i = 0; i < size; i++)
	{
		Transform t = getGlobalTransform(i);
		out[i] = transform2Matrix(t);
	}
}

bool Pose::operator==(const Pose& other)
{
	if (mJoints.size() != other.mJoints.size()) {
		return false;
	}
	if (mParents.size() != other.mParents.size()) {
		return false;
	}
	unsigned int size = (unsigned int)mJoints.size();
	for (unsigned int i = 0; i < size; ++i) {
		Transform thisLocal = mJoints[i];
		Transform otherLocal = other.mJoints[i];

		int thisParent = mParents[i];
		int otherParent = other.mParents[i];

		if (thisParent != otherParent) {
			return false;
		}

		if (thisLocal != otherLocal) {
			return false;
		}
	}
	return true;
}

bool Pose::operator!=(const Pose& other)
{
	return !(*this == other);
}
