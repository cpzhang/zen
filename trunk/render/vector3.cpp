#include "vector3.h"

const Vector3 Vector3::Zero(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::One(1.0f, 1.0f, 1.0f);
const Vector3 Vector3::AxisX(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::AxisY(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::AxisZ(0.0f, 0.0f, 1.0f);
Vector3::Vector3()
{

}

Vector3::Vector3( float a, float b, float c ) :D3DXVECTOR3( a, b, c)
{

}

Vector3::Vector3( const D3DXVECTOR3& v )
{
	*static_cast< D3DXVECTOR3 * >( this ) = v;
}

Vector3::Vector3( const float fX ) : D3DXVECTOR3( fX , fX , fX )
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

Vector3& Vector3::operator=( const Vector3& rkVector )
{
	x = rkVector.x;
	y = rkVector.y;
	z = rkVector.z;            

	return *this;
}

float Vector3::length() const
{
	return D3DXVec3Length(this);
}

float Vector3::lengthSquared() const
{
	return D3DXVec3LengthSq(this);
}

ApiRender_ Vector3 operator+( const Vector3& v1, const Vector3& v2 )
{
	return Vector3( v1.x+v2.x, v1.y+v2.y, v1.z+v2.z );
}

ApiRender_ Vector3 operator-( const Vector3& v1, const Vector3& v2 )
{
	return Vector3( v1.x-v2.x, v1.y-v2.y, v1.z-v2.z );
}
ApiRender_ bool operator   ==( const Vector3& v1, const Vector3& v2 )
{
	return ( v1[0] == v2[0] ) & ( v1[1] == v2[1] ) & ( v1[2] == v2[2] );
}
ApiRender_ Vector3 operator *( const Vector3& a, const Vector3& b )
{
	return Vector3( a.x*b.x, a.y*b.y, a.z*b.z );
}
ApiRender_ Vector3 operator *( const Vector3& v, float s )
{
	return Vector3( v.x * s, v.y * s, v.z * s );
}
ApiRender_ Vector3 operator *( float s, const Vector3& v )
{
	return Vector3( v.x * s, v.y * s, v.z * s );
}