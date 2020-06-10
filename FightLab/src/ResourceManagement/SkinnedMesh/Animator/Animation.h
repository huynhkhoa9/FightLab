#pragma once
#include "Utility.h"

struct AnimationSampler
{
	std::string            interpolation;
	std::vector<float>     inputs;
	std::vector<glm::vec4> outputsVec4;
};

struct AnimationChannel
{
	std::string path;
	Node* node;
	uint32_t    samplerIndex;
};

struct Animation
{
	std::string name;
	std::vector<AnimationSampler> samplers;
	std::vector<AnimationChannel> channels;
	float                         start = std::numeric_limits<float>::max();
	float                         end = std::numeric_limits<float>::min();
	float                         CurrentTime;
};