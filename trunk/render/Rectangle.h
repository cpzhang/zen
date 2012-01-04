#pragma once
//
#include "misc/dxHead.h"
#include "misc/helper.h"
#include "ConfigRender.h"
#include "math.h"
class ApiRender_ RectangleT
{
public:
	RectangleT();
	RectangleT(float l, float r, float t, float b);
	~RectangleT();
public:
	//判断是否相交
	bool isIntersected(const RectangleT* rc)
	{
		if (rc == NULL || *rc == RectangleT::Zero)
		{
			return false;
		}
		if (rc->left_ >= right_ || rc->top_ <= bottom_ || rc->right_ <= left_ || rc->bottom_ >= top_)
		{
			return false;
		}
		return true;
	}
	//求交
	RectangleT getIntersection(const RectangleT* rc)
	{
		RectangleT rt = RectangleT::Zero;
		rt.left_ = max(rc->left_, left_);
		rt.bottom_ = max(rc->bottom_, bottom_);
		rt.right_ = min(rc->right_, right_);
		rt.top_ = min(rc->top_, top_);
		return rt;
	}
	bool operator == (const RectangleT& rc) const
	{
		if (almostEqual(rc.left_, left_) && almostEqual(rc.top_, top_) && almostEqual(rc.bottom_, bottom_) && almostEqual(rc.top_, top_))
		{
			return true;
		}
		return false;
	}
public:
	float left_;
	float right_;
	float bottom_;
	float top_;
public:
	static RectangleT Zero;
};