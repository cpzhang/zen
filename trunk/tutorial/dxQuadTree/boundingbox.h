#pragma once
class BoundingBox
{
public:
	BoundingBox()
	: min_( FLT_MAX, FLT_MAX, FLT_MAX ),
	max_( -FLT_MAX, -FLT_MAX, -FLT_MAX )
  {

  }
	BoundingBox( const Vector3 & min, const Vector3 & max )
		:min_( min ),
		max_( max )
	{

	}
	const Vector3 & minBounds() const
	{
		return min_;
	}
	const Vector3 & maxBounds() const
	{
		return max_;
	}
private:
	Vector3 min_;
	Vector3 max_;
};