#pragma once
#include <string>
#include <vector>
#include "Graphics/RenderBackEnd/Utility.h"

struct SpriteUBO {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
	glm::vec2 spriteOffset;
	glm::vec2 scale;
};

struct FrameInfo
{
	glm::vec3 scale = glm::vec3(0);
	glm::vec3 offset = glm::vec3(0);
};

struct Sprite
{
	glm::vec3 Position;
	glm::vec2 Scale;

	std::string Texture;

	Vertex vertices[4]{
		{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
		{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
		{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}}
	};
	
	std::vector<FrameInfo> FramesData;

	const std::array<uint16_t, 6> indices = {
		0, 1, 2, 2, 3, 0
	};
};