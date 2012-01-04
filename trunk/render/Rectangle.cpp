#include "Rectangle.h"

RectangleT::RectangleT()
{
	left_ = right_ = bottom_ = top_ = 0.0f;
}

RectangleT::RectangleT( float l, float r, float t, float b )
{
	left_= l;
	right_ = r;
	top_ = t;
	bottom_ = b;
}

RectangleT::~RectangleT()
{

}

ApiRender_ RectangleT RectangleT::Zero(0, 0, 0, 0);
