#pragma once
#include <glm.hpp>
#include <gtx/quaternion.hpp>
#include <gtc/quaternion.hpp>
struct Transform
{
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

	Transform()
	{
		position = glm::vec3(0);
		rotation = glm::quat(0, 0, 0, 1);
		scale = glm::vec3(1,1,1);

	}
	Transform(const glm::vec3& p, const glm::quat& r, const glm::vec3& s)
	{
		position = p;
		rotation = r;
		scale = s;
	}
};

bool operator==(const Transform& a, const Transform& b);
bool operator!=(const Transform& a, const Transform& b);

Transform combine(const Transform& t1, const Transform& t2);

Transform inverse(const Transform& t);

Transform mix(const Transform& t1, const Transform& t2, const float& t);

glm::mat4 transform2Matrix(const Transform& t);

Transform mat2Transform(const glm::mat4& m);

glm::vec3 tranformPoint(const Transform& t, const glm::vec3& p);

glm::vec3 transformVector(const Transform& t, const glm::vec3& v);