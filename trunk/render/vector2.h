#pragma once
//
#include "common/dxHead.h"
#include "misc/helper.h"
#include "ConfigRender.h"
class ApiRender_ Vector2 : public D3DXVECTOR2
{
public:
	Vector2();
	Vector2( float a, float b);
public:
	static Vector2 Zero;
};