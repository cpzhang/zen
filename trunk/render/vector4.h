#pragma once
//
#include "common/dxHead.h"
#include "misc/helper.h"
#include "ConfigRender.h"
#include "vector3.h"
class ApiRender_ Vector4 : public D3DXVECTOR4
{
	//(r,g,b,a)(x,y,z,w)
public:
	Vector4();
	Vector4(const Vector3& v);
	Vector4( float a, float b, float c, float d );
public:
	float Vector4::lengthSquared() const
	{
		return D3DXVec4LengthSq( this );
	}
	float Vector4::length() const
	{
		return D3DXVec4Length( this );
	}
	u32 getARGB() const;
	inline Vector4 operator + ( const Vector4& rkVector ) const
	{
		return Vector4(
			x + rkVector.x,
			y + rkVector.y,
			z + rkVector.z,
			w + rkVector.w);
	}
	void normalize()
	{
		D3DXVec4Normalize(this, this);
	}
public:
	static const Vector4 Zero;
	static const Vector4 One;
};

ApiRender_ Vector4 operator *( const Vector4& v, float s );
ApiRender_ Vector4 operator *( float s, const Vector4& v );
