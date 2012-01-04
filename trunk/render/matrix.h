#pragma once
#include "misc/dxHead.h"
#include "vector4.h"
#include "vector3.h"
#include "ConfigRender.h"
class Quaternion;
class ApiRender_ Matrix : public D3DXMATRIX
{
public:
	Matrix();

	Matrix( const Vector4& v0,
		const Vector4& v1,
		const Vector4& v2,
		const Vector4& v3 );
public:
	void preMultiply( const Matrix& m )
	{
		D3DXMatrixMultiply( this, &m, this );
	}
	void postMultiply( const Matrix& m )
	{
		D3DXMatrixMultiply( this, this , &m);
	}

	void setTranslate( const float x, const float y, const float z )
	{
		D3DXMatrixTranslation( this, x, y, z );
	}
	void setTranslate( const Vector3 & pos )
	{
		setTranslate( pos.x, pos.y, pos.z );
	}

	void setIdentity()
	{
		D3DXMatrixIdentity( this );	
	}

	Vector3 & operator []( int i )
	{
		return *reinterpret_cast<Vector3 *>( m[i] );
	}
	const Vector3 & Matrix::operator []( int i ) const
	{
		return *reinterpret_cast< const Vector3 * >( m[i] );
	}

	const Vector3 & applyToOrigin() const
	{
		return (*this)[3];
	}

	void setZero()
	{
		memset( this, 0, sizeof( *this ) );
	}
	void setRow( int i, const Vector4& value )
	{
		m[i][0] = value.x;
		m[i][1] = value.y;
		m[i][2] = value.z;
		m[i][3] = value.w;
	}
	Vector4 getRow( int i)
	{
		return Vector4(m[i][0], m[i][1], m[i][2], m[i][3]);
	}
	Vector4 getColumn( int i)
	{
		return Vector4(m[0][i], m[1][i], m[2][i], m[3][i]);
	}
	void setColumn( int i, const Vector4& value )
	{
		m[0][i] = value.x;
		m[1][i] = value.y;
		m[2][i] = value.z;
		m[3][i] = value.w;
	}
	void lookAt( const Vector3& eye, const Vector3& at, const Vector3& up )
	{
		D3DXMatrixLookAtLH(this, &eye, &at, &up);
	}
	bool invert()
	{
		return (D3DXMatrixInverse( this, NULL, this ) != NULL);
	}
	bool invert( const Matrix & matrix )
	{
		return (D3DXMatrixInverse( this, NULL, &matrix ) != NULL);
	}
	Matrix getInverse()
	{
		Matrix m;
		D3DXMatrixInverse( &m, NULL, this);
		return m;
	}
	const Vector3 & applyToUnitAxisVector( int axis ) const
	{
		return *reinterpret_cast<const Vector3 *>( m[axis] );
	}
	void perspectiveProjection( float fov, float aspectRatio, float nearPlane, float farPlane )
	{
		D3DXMatrixPerspectiveFovLH( this, fov, aspectRatio, nearPlane, farPlane );
	}
	void multiply( const Matrix& m1, const Matrix& m2 )
	{
		D3DXMatrixMultiply( this, &m1, &m2 );
	}
	void setScale( const float x, const float y, const float z )
	{
		D3DXMatrixScaling( this, x, y, z );
	}
	void setScale( const Vector3 & scale )
	{
		setScale( scale.x, scale.y, scale.z );
	}
	void	setRotate( Quaternion* q )
	{
		D3DXMatrixRotationQuaternion( this, (const D3DXQUATERNION *)q );
	}
	void make( const Vector3& position, const Vector3& scale, Quaternion& orientation )
	{
		setIdentity();
		setScale(scale);
		setRotate(&orientation);
		setTranslate(position);
	}
	Vector3 applyVector( const Vector3& v2 ) const
	{
		Vector3 vout;
		D3DXVec3TransformNormal( &vout, &v2, this );
		return vout;
	}

public:
	static const Matrix Identity;
};
ApiRender_ Matrix operator *( const Matrix& a, const Matrix& b );