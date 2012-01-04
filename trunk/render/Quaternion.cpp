#include "Quaternion.h"
void Quaternion::set( const Vector3 &v, float _w )
{
	this->x	    = v.x;
	this->y     = v.y;
	this->z     = v.z;
	this->w		= _w;
}

Quaternion::Quaternion()
{
}


Quaternion::Quaternion( const Matrix &m )
{
	this->fromMatrix( m );
}


Quaternion::Quaternion( float x, float y, float z, float w ) :
D3DXQUATERNION( x, y, z, w )
{
}


Quaternion::Quaternion( const Vector3 &v, float w )
{
	this->set( v, w );
}


void Quaternion::setZero()
{
	x = y = z = w = 0.0f;
}


void Quaternion::set( float _x, float _y, float _z, float _w )
{
	this->x	    = _x;
	this->y     = _y;
	this->z     = _z;
	this->w		= _w;
}

void Quaternion::preMultiply( const Quaternion& q )
{
	this->multiply( q, *this );
}


void Quaternion::postMultiply( const Quaternion& q )
{
	this->multiply( *this, q );
}

float Quaternion::dotProduct( const Quaternion& q ) const
{
	return D3DXQuaternionDot( this, &q );
}

float Quaternion::length() const
{
	Vector4 vec4(x, y, z, w);
	return vec4.length();
}

float Quaternion::lengthSquared() const
{
	Vector4 vec4(x, y, z, w);
	return vec4.lengthSquared();
}

Quaternion operator *( const Quaternion& q1, const Quaternion& q2 )
{
	Quaternion q;
	q.multiply( q1, q2 );
	return q;
}


bool operator ==( const Quaternion& q1, const Quaternion& q2 )
{
	return ( q1.x == q2.x && 
	         q1.y == q2.y &&
	         q1.z == q2.z &&
	         q1.w == q2.w );
}

void Quaternion::minimise()
{
	if (w < 0)
	{
		x = -x;
		y = -y;
		z = -z;
		w = -w;
	}
}

void Quaternion::slerp( const Quaternion &qStart, const Quaternion &qEnd,
		float t )
{
	D3DXQuaternionSlerp( this, &qStart, &qEnd, t );
}


void Quaternion::fromAngleAxis( float angle, const Vector3 &axis )
{
	D3DXQuaternionRotationAxis( this, &axis, angle );
}


void Quaternion::fromMatrix( const Matrix &m )
{
	D3DXQuaternionRotationMatrix( this, &m );
}

void Quaternion::multiply( const Quaternion& q1, const Quaternion& q2 )
{
	D3DXQuaternionMultiply( this, &q1, &q2 );
}

void Quaternion::normalise()
{
	D3DXQuaternionNormalize( this, this );
}

void Quaternion::invert()
{
	D3DXQuaternionInverse( this, this );
}