#pragma once
#include "ConfigRender.h"
#include "vector3.h"
ApiRender_ inline bool almostEqual( const float f1, const float f2, const float epsilon = 0.0004f )
{
	return fabsf( f1 - f2 ) < epsilon;
}

ApiRender_ inline bool almostEqual( const Vector3 f1, const Vector3 f2, const float epsilon = 0.0004f )
{
	return almostEqual(f1.x, f2.x, epsilon) && almostEqual(f1.z, f2.z, epsilon) && almostEqual(f1.z, f2.z, epsilon);
}

template <class TYPE>
inline TYPE clamp( TYPE minValue, TYPE value, TYPE maxValue )
{
	return value < minValue ? minValue :
		( value > maxValue ? maxValue : value);
}

/// This value defines pi.
const float MATH_PI = 3.14159265359f;
const float MATH_PI_Half = MATH_PI/2.0f;
const float MATH_PI_Two = MATH_PI * 2.0f;
inline float DEG_TO_RAD( float angle )
{
	return angle * MATH_PI / 180.f; 
}