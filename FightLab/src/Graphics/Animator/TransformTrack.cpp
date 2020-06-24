#include "pch.h"
#include "TransformTrack.h"

TransformTrack::TransformTrack()
{
	mID = 0;
}

unsigned int TransformTrack::GetID()
{
	return mID;
}

void TransformTrack::SetID(unsigned int id)
{
	mID = id;
}

VectorTrack& TransformTrack::getPositionTrack()
{
	return mPosition;
}

QuaternionTrack& TransformTrack::getRotationTrack()
{
	return mRotation;
}

VectorTrack& TransformTrack::getScaleTrack()
{
	return mScale;
}

float TransformTrack::getStartTime()
{
	float result = 0;
	bool isSet = false;
	if (mPosition.Size() > 1)
	{
		result = mPosition.getStartTime();
		isSet = true;
	}

	if (mRotation.Size() > 1)
	{
		float rotationStart = mRotation.getStartTime();
		if (rotationStart < result || !isSet)
		{
			result = rotationStart;
			isSet = true;
		}
	}

	if (mScale.Size() > 1)
	{
		float scaleStart = mScale.getStartTime();
		if (scaleStart < result || !isSet)
		{
			result = scaleStart;
			isSet = true;
		}
	}

	return result;
}

float TransformTrack::getEndTime()
{
	float result = 0;
	bool isSet = false;
	if (mPosition.Size() > 1)
	{
		result = mPosition.getEndTime();
		isSet = true;
	}

	if (mRotation.Size() > 1)
	{
		float rotationEnd = mRotation.getEndTime();
		if (rotationEnd > result || !isSet)
		{
			result = rotationEnd;
			isSet = true;
		}
	}

	if (mScale.Size() > 1)
	{
		float scaleEnd = mScale.getEndTime();
		if (scaleEnd > result || !isSet)
		{
			result = scaleEnd;
			isSet = true;
		}
	}

	return result;
}

bool TransformTrack::isValid()
{
	return mPosition.Size() > 1 ||
		   mRotation.Size() > 1 ||
		   mScale.Size() > 1;
}

Transform TransformTrack::Sample(const Transform& ref, float time, bool looping)
{
	Transform result = ref;
	if (mPosition.Size() > 1)
	{
		result.position = mPosition.Sample(time, looping);
	}
	if (mRotation.Size() > 1)
	{
		result.rotation = mRotation.Sample(time, looping);
	}
	if (mScale.Size() > 1)
	{
		result.scale = mScale.Sample(time, looping);
	}

	return result;
}
