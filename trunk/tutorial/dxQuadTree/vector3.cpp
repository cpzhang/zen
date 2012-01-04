#include "vector3.h"

const Vector3 Vector3::Zero(0.0f, 0.0f, 0.0f);

Vector3::Vector3()
{

}

Vector3::Vector3( float a, float b, float c ) :D3DXVECTOR3( a, b, c)
{

}

Vector3 Vector3::operator-() const
{
	Vector3 that;
	that.x = -x;
	that.y = -y;
	that.z = -z;
	return that;
}

void Vector3::normalise()
{
	D3DXVec3Normalize( this, this );
}

Vector3 Vector3::unitVector() const
{
	Vector3 result( *this );
	result.normalise();
	return result;
}

Vector3 operator+( const Vector3& v1, const Vector3& v2 )
{
	return Vector3( v1.x+v2.x, v1.y+v2.y, v1.z+v2.z );
}

Vector3 operator-( const Vector3& v1, const Vector3& v2 )
{
	return Vector3( v1.x-v2.x, v1.y-v2.y, v1.z-v2.z );
}
