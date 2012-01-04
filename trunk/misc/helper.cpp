#include "helper.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cmath>
#include <cstdlib>
ApiMisc_ bool isKeyDown( int vk )
{
	return (::GetAsyncKeyState(vk) & 0x8001);
}

ApiMisc_ float randomReal( const float left, const float right, const unsigned int random_max/* = 0x7fff*/ )
{
	float r = (float)(rand() % random_max);
	r /= random_max;

	return left*r + (1-r)*right;
}
