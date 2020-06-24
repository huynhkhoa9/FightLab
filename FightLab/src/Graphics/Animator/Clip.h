#pragma once
#include "TransformTrack.h"
#include "Pose.h"

class Clip
{
protected:
	std::vector<TransformTrack> mTracks;
	std::string mName;
	float mStartTime;
	float mEndTime;
	bool mLooping;

	float adjustTimeToFitRange(float inTime);
public:
	Clip();
	unsigned int getIDAtIndex(unsigned int index);
	void setIDAtIndex(unsigned int idx, unsigned int id);

	unsigned int size();
	float sample(Pose& outPose, float inTime);
	TransformTrack& operator[](unsigned int joint);

	void recalculateDuration();

	std::string& getName();
	void setName(const std::string& inNewName);
	float getDuration();
	float getStartTime();
	float getEndTime();
	bool getLooping();
	void setLooping(bool inLooping);
};

