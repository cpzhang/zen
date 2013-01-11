#include "vector4.h"
#include "Colour.h"
Vector4::Vector4()
{

}

Vector4::Vector4( float a, float b, float c, float d ) :D3DXVECTOR4( a, b, c, d )
{

}

Vector4::Vector4( const Vector3& v ):D3DXVECTOR4( v.x, v.y, v.z, 0.0f )
{

}

u32 Vector4::getARGB()
{
	return Colour::getUint32FromNormalised(*this);
}

const Vector4 Vector4::Zero(0.0f, 0.0f, 0.0f, 0.0f);
const Vector4 Vector4::One(1.0f, 1.0f, 1.0f, 1.0f);

ApiRender_ Vector4 operator*( const Vector4& v, float s )
{
	return Vector4( v.x * s, v.y * s, v.z * s , v.w * s);
}

ApiRender_ Vector4 operator*( float s, const Vector4& v )
{
	return Vector4( v.x * s, v.y * s, v.z * s , v.w * s);
}
