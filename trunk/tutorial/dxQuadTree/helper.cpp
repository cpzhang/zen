#include "helper.h"
#include "windowsHead.h"
bool isKeyDown( int vk )
{
	return (::GetAsyncKeyState(vk) & 0x8001);
}
