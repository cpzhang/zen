#pragma once
#include "ConfigRender.h"
#include "Vector3.h"
#include "Matrix.h"
//
class ApiRender_ Quaternion : public D3DXQUATERNION
{
	//
public:
	Quaternion();
	Quaternion( const Matrix &m );
	Quaternion( float x, float y, float z, float w );
	Quaternion( const Vector3 &v, float w );
	//@}

	void			setZero();
	void			set( float x, float y, float z, float w );
	void			set( const Vector3 &v, float w );

	void			fromAngleAxis( float angle, const Vector3 &axis );

	void			fromMatrix( const Matrix &m );

	void			normalise();
	void			invert();
	void			minimise();

	void			slerp( const Quaternion& qStart, const Quaternion &qEnd,
		float t );

	void			multiply( const Quaternion& q1, const Quaternion& q2 );
	void			preMultiply( const Quaternion& q );
	void			postMultiply( const Quaternion& q );

	float			dotProduct( const Quaternion& q ) const;
	float			length() const;
	float			lengthSquared() const;
	inline Quaternion operator + (const Quaternion& q) const
	{
		return Quaternion(x + q.x, y + q.y, z + q.z, w + q.w);
	}
	inline Quaternion operator * (const float scale) const
	{
		return Quaternion(x * scale, y * scale, z * scale, w * scale);
	}

	//
	inline friend Quaternion operator* (float scale, const Quaternion& q)
	{
		return q * scale;
	}
};

Quaternion  operator *( const Quaternion& q1, const Quaternion& q2 );
bool		operator ==( const Quaternion& q1, const Quaternion& q2 );