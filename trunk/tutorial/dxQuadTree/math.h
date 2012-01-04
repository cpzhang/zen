#pragma once
bool almostEqual( const float f1, const float f2, const float epsilon = 0.0004f )
{
	return fabsf( f1 - f2 ) < epsilon;
}


template <class TYPE>
inline TYPE clamp( TYPE minValue, TYPE value, TYPE maxValue )
{
	return value < minValue ? minValue :
		( value > maxValue ? maxValue : value);
}