#pragma once
#include <string>
#include "Graphics/RenderBackEnd/Vertex.h"

struct Sprite
{
	glm::vec3 Position;
	glm::vec2 Scale;

	std::string Texture;

	Vertex vertices[4]{};
	const std::array<uint16_t, 6> indices = {
		0, 1, 2, 2, 3, 0
	};
};