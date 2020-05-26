#include "pch.h"
#include "Animation.h"

Animation::Animation(uint16_t duration /*=0*/)
{
	CurrentTime = 0;
	Duration = duration;
}

Animation::~Animation()
{
}

void Animation::Play()
{
	if (CurrentTime < Duration)
		CurrentTime++;
}
