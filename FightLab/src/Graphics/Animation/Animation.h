#pragma once
class Animation
{
public:
	Animation(uint16_t duration = 0);
	~Animation();
	
	//in frames
	uint16_t Duration;

	//in frames since the animation started
	uint16_t CurrentTime;

	void Play();
};

