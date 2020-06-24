#pragma once
#include "Interpolation.h"
#include "Frame.h"

template<typename T, unsigned int N>
class Track
{
protected:
	std::vector<Frame<N>> mFrames;
	Interpolation mInterpolation;

	T SampleConstant(float time, bool looping);
	T SampleLinear(float time, bool looping);
	T SampleCubic(float time, bool looping);

	T Hermite(float time, const T& p1, const T& s1, const T& p2, const T& s2);

	int FrameIndex(float time, bool looping); 
	float AdjustTimeToFitTrack(float t, bool loop);

	T Cast(float* value); // Will be specialized

public:
	Track();
	void Resize(unsigned int size);
	unsigned int Size();
	Interpolation getInterpolation();
	void setInterpolation(Interpolation interpolation);
	float getStartTime();
	float getEndTime();

	T Sample(float time, bool looping);
	Frame<N>& operator[](unsigned int index);
};

typedef Track<float, 1> ScalarTrack;
typedef Track<glm::vec3, 3> VectorTrack;
typedef Track<glm::quat, 4> QuaternionTrack;


