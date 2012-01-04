#pragma once
//
#include "misc/dxHead.h"
#include "misc/helper.h"
#include "ConfigRender.h"
class ApiRender_ Vector3 : public D3DXVECTOR3
{
public:
	Vector3();
	Vector3( float a, float b, float c );
	Vector3( const float fX);
	explicit Vector3( const D3DXVECTOR3& v );
	Vector3 operator-() const;
	void normalise();
	Vector3& operator = ( const Vector3& rkVector );
	float length() const;
	float lengthSquared() const;
	Vector3 unitVector() const;
	float dotProduct( const Vector3& v ) const
	{
		return D3DXVec3Dot( this, &v );
	};
	void crossProduct( const Vector3& v1, const Vector3& v2 )
	{
		D3DXVec3Cross( this, &v1, &v2 );
	}

public:
	static const Vector3 Zero;
	static const Vector3 AxisX;
	static const Vector3 AxisY;
	static const Vector3 AxisZ;
	static const Vector3 One;
};

ApiRender_ Vector3 operator +( const Vector3& v1, const Vector3& v2 );
ApiRender_ Vector3 operator -( const Vector3& v1, const Vector3& v2 );
ApiRender_ bool operator   ==( const Vector3& v1, const Vector3& v2 );
ApiRender_ Vector3 operator *( const Vector3& v, float s );
ApiRender_ Vector3 operator *( float s, const Vector3& v );
ApiRender_ Vector3 operator *( const Vector3& a, const Vector3& b );
