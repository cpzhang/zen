#include "Ray.h"

Ray::Ray()
{
	origion_ = Vector3::Zero;
	direction_ = Vector3::AxisZ;
}

bool Ray::getIntersetTriangleParameters( const Vector3& p0, const Vector3& p1, const Vector3& p2, Vector3& ps )
{
	return D3DXIntersectTri(&p0, &p1, &p2, &origion_, &direction_, &ps.x, &ps.y, &ps.z);
}
