#pragma once
#include "Track.h"
#include "Core/Transform.h"
class TransformTrack
{
protected:
	unsigned int mID;
	VectorTrack mPosition;
	QuaternionTrack mRotation;
	VectorTrack mScale;
public:
	TransformTrack();
	unsigned int GetID();
	void SetID(unsigned int id);
	VectorTrack& getPositionTrack();
	QuaternionTrack& getRotationTrack();
	VectorTrack& getScaleTrack();

	float getStartTime();
	float getEndTime();

	bool isValid();
	
	Transform Sample(const Transform& ref, float time, bool looping);
};

