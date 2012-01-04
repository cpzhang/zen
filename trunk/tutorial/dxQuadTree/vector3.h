#pragma once
//
#include "dxHead.h"
class Vector3 : public D3DXVECTOR3
{
public:
	Vector3();
	Vector3( float a, float b, float c );
	explicit Vector3( const D3DXVECTOR3& v )
	{
		*static_cast< D3DXVECTOR3 * >( this ) = v;
	}
	Vector3 operator-() const;
	void normalise();

	Vector3 unitVector() const;
	static const Vector3 Zero;
};

Vector3 operator +( const Vector3& v1, const Vector3& v2 );
Vector3 operator -( const Vector3& v1, const Vector3& v2 );
