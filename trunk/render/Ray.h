#pragma once
#include "vector3.h"
#include "misc/helper.h"
class ApiRender_ Ray
{
public:
	Ray();
	bool getIntersetTriangleParameters( const Vector3& p0, const Vector3& p1, const Vector3& p2, Vector3& ps );
	Vector3 getPoint(float t)
	{
		return origion_ + t * direction_;
	}
public:
	Vector3 origion_;
	Vector3 direction_;
};