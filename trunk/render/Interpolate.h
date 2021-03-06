#pragma once
#include "Quaternion.h"
//#include "d3dx9math.h"
#include "common/dxHead.h"
/** Interpolation type. */
enum eInterpolationType
{
	eInterpolationType_None = 0,
	eInterpolationType_Bezier_Linear,
	eInterpolationType_Bezier_Quadratic,
	eInterpolationType_Bezier_Cubic,
	INTERPOLATION_HERMITE,
	eInterpolationType_Max,
	eForce = 0x7fffffff
};

// the degree-n Bézier curve is a linear interpolation between two degree-(n − 1) Bézier curves.

/** eInterpolationType_Bezier_Linear线性插值
*/
template<class T>
inline T InterpolateBezier(const float r,const T &v1,const T &v2)
{
	T v = v1 * (1.0f - r) + v2 * r;
	return v;
}


/** eInterpolationType_Bezier_Quadratic插值
*/
template<class T>
inline T InterpolateBezier(const float t, const T &p0,const T &p1,const T &p2)
{
	return (1 - t) * ((1 - t) * p0 + t * p1) + t * ((1 - t) * p1 + t * p2);
}

/** eInterpolationType_Bezier_Cubic插值
*/
template<class T>
inline T InterpolateBezier(const float t, const T &p0,const T &p1,const T &p2, const T &p3)
{
	return (1 - t) * ((1 - t) * ((1 - t) * p0 + t * p1) + t * ((1 - t) * p1 + t * p2)) + t * ((1 - t) * ((1 - t) * p1 + t * p2) + t * ((1 - t) * p2 + t * p3));
}

/** Hermite插值
*/
template<class T>
inline T InterpolateHermite(const float Factor,const T &v1,const T &v2,const T &in,const T &out)
{
	float Factor1;
	float Factor2;
	float Factor3;
	float Factor4;
	float FactorTimesTwo;

	FactorTimesTwo = Factor * Factor;

	Factor1 = FactorTimesTwo * (2.0f * Factor - 3.0f) + 1;
	Factor2 = FactorTimesTwo * (Factor - 2.0f) + Factor;
	Factor3 = FactorTimesTwo * (Factor - 1.0f);
	Factor4 = FactorTimesTwo * (3.0f - 2.0f * Factor);

	return (Factor1 * v1) + (Factor2 * out) + (Factor3 * in) + (Factor4 * v2);

	float r = Factor;
	float r2 = r * r;
	float r3 = r2 * r;
	float r2_3 = r2 * 3.0f;
	float r3_2 = 2.0f * r3;

	float h1 = r3_2 - r2_3 + 1.0f;
	float h2 = -r3_2 + r2_3;
	float h3 = r3 - 2.0f * r2 + r;
	float h4 = r3 - r2;

	// interpolation
	T v = v1 * h1 + v2 * h2 + in * h3 + out * h4;
	return v;
}

/** 四元数Sphere插值
*/
template<>
inline Quaternion InterpolateBezier<Quaternion>(const float r,const Quaternion &v1,const Quaternion &v2)
{
	Quaternion q;
	D3DXQuaternionSlerp(&q, &v1, &v2, r);
	return q;
}