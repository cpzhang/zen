#include "boundingbox.h"
#include "common/dxHead.h"
BoundingBox::BoundingBox() : min_( FLT_MAX, FLT_MAX, FLT_MAX ),
max_( -FLT_MAX, -FLT_MAX, -FLT_MAX )
{

}

BoundingBox::BoundingBox( const Vector3 & min, const Vector3 & max ) :min_( min ),
max_( max )
{

}

const Vector3 & BoundingBox::minBounds() const
{
	return min_;
}

const Vector3 & BoundingBox::maxBounds() const
{
	return max_;
}

bool BoundingBox::intersectsRay( const Ray & r) const
{
	return D3DXBoxBoundProbe(&min_, &max_, &r.origion_, &r.direction_);
}

void BoundingBox::render()
{

}
