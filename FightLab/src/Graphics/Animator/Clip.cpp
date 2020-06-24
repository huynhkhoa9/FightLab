#include "pch.h"
#include "Clip.h"

float Clip::adjustTimeToFitRange(float inTime)
{
	if (mLooping) {
		float duration = mEndTime - mStartTime;
		if (duration <= 0) {
			return 0.0f;
		}
		inTime = fmodf(inTime - mStartTime, mEndTime - mStartTime);
		if (inTime < 0.0f) {
			inTime += mEndTime - mStartTime;
		}
		inTime = inTime + mStartTime;
	}
	else {
		if (inTime < mStartTime) {
			inTime = mStartTime;
		}
		if (inTime > mEndTime) {
			inTime = mEndTime;
		}
	}
	return inTime;
}

Clip::Clip()
{
	mName = "No name give";
	mStartTime = 0.0f;
	mEndTime = 0.0f;
	mLooping = true;
}

unsigned int Clip::getIDAtIndex(unsigned int index)
{
	return mTracks[index].GetID();;
}

void Clip::setIDAtIndex(unsigned int idx, unsigned int id)
{
	return mTracks[idx].SetID(id);
}

unsigned int Clip::size()
{
	return (unsigned int)mTracks.size();
}

float Clip::sample(Pose& outPose, float inTime)
{
	if (getDuration() == 0.0f)
	{
		return 0.0f;
	}
	
	inTime = adjustTimeToFitRange(inTime);
	unsigned int size = mTracks.size();
	for (unsigned int i = 0; i < size; ++i) {
		unsigned int joint = mTracks[i].GetID();
		Transform local = outPose.getLocalTransform(joint);
		Transform animated = mTracks[i].Sample(local, inTime, mLooping);
		outPose.setLocalTransform(joint, animated);
	}
	return inTime;
}

TransformTrack& Clip::operator[](unsigned int joint)
{
	for (unsigned int i = 0, size = (unsigned int)mTracks.size();
		i < size; ++i) {
		if (mTracks[i].GetID() == joint) {
			return mTracks[i];
		}
	}

	mTracks.push_back(TransformTrack());
	mTracks[mTracks.size() - 1].SetID(joint);
	return mTracks[mTracks.size() - 1];
}

void Clip::recalculateDuration()
{
	mStartTime = 0.0f;
	mEndTime = 0.0f;
	bool startSet = false;
	bool endSet = false;
	unsigned int tracksSize = (unsigned int)mTracks.size();
	for (unsigned int i = 0; i < tracksSize; ++i) {
		if (mTracks[i].isValid()) {
			float trackStartTime = mTracks[i].getStartTime();
			float trackEndTime = mTracks[i].getEndTime();

			if (trackStartTime < mStartTime || !startSet) {
				mStartTime = trackStartTime;
				startSet = true;
			}

			if (trackEndTime > mEndTime || !endSet) {
				mEndTime = trackEndTime;
				endSet = true;
			}
		}
	}
}

std::string& Clip::getName()
{
	return mName;
}

void Clip::setName(const std::string& inNewName)
{
	mName = inNewName;
}

float Clip::getDuration()
{
	return mEndTime - mStartTime;
}

float Clip::getStartTime()
{
	return mStartTime;
}

float Clip::getEndTime()
{
	return mEndTime;
}

bool Clip::getLooping()
{
	return mLooping;
}

void Clip::setLooping(bool inLooping)
{
	mLooping = inLooping;
}
