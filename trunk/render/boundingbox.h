#pragma once
#include "vector3.h"
#include "matrix.h"
#include "Ray.h"
#include "misc/helper.h"
class ApiRender_ BoundingBox
{
public:
	BoundingBox();
	BoundingBox( const Vector3 & min, const Vector3 & max );
	const Vector3 & minBounds() const;
	const Vector3 & maxBounds() const;
	bool intersectsRay( const Ray & r) const;
	void render(const Matrix& wm, u32 color);
	void addBounds( const Vector3 & v )
	{
		if (min_[0] > v[0])
			min_[0] = v[0];

		if (min_[1] > v[1])
			min_[1] = v[1];

		if (min_[2] > v[2])
			min_[2] = v[2];

		if (max_[0] < v[0])
			max_[0] = v[0];

		if (max_[1] < v[1])
			max_[1] = v[1];

		if (max_[2] < v[2])
			max_[2] = v[2];
	}
public:
	Vector3 min_;
	Vector3 max_;
};