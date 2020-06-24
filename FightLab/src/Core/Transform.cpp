#include "pch.h"
#include "Transform.h"

#define VEC3_EPSILON 0.000001f

bool operator==(const Transform& a, const Transform& b) {
	return a.position == b.position &&
		a.rotation == b.rotation &&
		a.scale == b.scale;
}

bool operator!=(const Transform& a, const Transform& b) {
	return !(a == b);
}

Transform combine(const Transform& t1, const Transform& t2)
{
	Transform out;
	out.scale = t1.scale * t2.scale;
	out.rotation = t1.rotation * t2.rotation;
	out.position = t1.rotation * (t1.scale * t2.position);
	out.position = t1.position + out.position;
	return out;
}

Transform inverse(const Transform& t)
{
	Transform inv;

	inv.rotation = glm::inverse(t.rotation); 
	inv.scale.x = fabs(t.scale.x) < VEC3_EPSILON ? 0.0f : 1.0f / t.scale.x;
	inv.scale.y = fabs(t.scale.y) < VEC3_EPSILON ? 0.0f : 1.0f / t.scale.y; 
	inv.scale.z = fabs(t.scale.z) < VEC3_EPSILON ? 0.0f : 1.0f / t.scale.z;
	glm::vec3 invTrans = t.position * -1.0f; 
	inv.position = inv.rotation * (inv.scale * invTrans);


	return inv;
}

Transform mix(const Transform& t1, const Transform& t2, const float& t)
{
	glm::quat bRot = t2.rotation; 
	
	if (glm::dot(t1.rotation, bRot) < 0.0f)
	{ 
		bRot = -bRot;
	} 
	
	return Transform(glm::lerp(t1.position, t2.position, t),
					 glm::lerp(t1.rotation, bRot, t),
					 glm::lerp(t1.scale, t2.scale, t));
}

glm::mat4 transform2Matrix(const Transform& t)
{
	glm::vec3 x = t.rotation * glm::vec3(1, 0, 0);
	glm::vec3 y = t.rotation * glm::vec3(0, 1, 0);
	glm::vec3 z = t.rotation * glm::vec3(0, 0, 1);

	x = x * t.scale.x;
	y = y * t.scale.y;
	z = z * t.scale.z;

	glm::vec3 p = t.position;

	return glm::mat4(x.x, x.y, x.z, 0,
					 y.x, y.y, y.z, 0, 
					 z.x, z.y, z.z, 0, 
					 p.x, p.y, p.z, 1);
}

Transform mat2Transform(const glm::mat4& m)
{
	Transform out;
	out.position = glm::vec3(m[3][0], m[3][1], m[3][2]);
	out.rotation = glm::toQuat(m);

	glm::mat4 rotScaleMat(m[0][0], m[0][1], m[0][2], 0,
						  m[1][0], m[1][1], m[1][2], 0, 
						  m[2][0], m[2][1], m[2][2], 0, 
						  0, 0, 0, 1 );
	glm::mat4 invRotMat = glm::toMat4(glm::inverse(out.rotation));

	glm::mat4 scaleSkewMat = rotScaleMat * invRotMat;
	out.scale = glm::vec3(scaleSkewMat[0][0], scaleSkewMat[1][1], scaleSkewMat[2][2] );
	return out;
}

glm::vec3 tranformPoint(const Transform& t, const glm::vec3& p)
{
	glm::vec3 out;

	out = t.rotation * (t.scale * p);
	out = t.position + out;

	return out;
}

glm::vec3 transformVector(const Transform& t, const glm::vec3& v)
{
	glm::vec3 out;

	out = t.rotation * (t.scale * v);

	return out;
}
