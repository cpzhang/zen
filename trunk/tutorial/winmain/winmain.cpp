#pragma once
#include "bootstrapper/BootStrapper.h"
#include "resource.h"
const LPCTSTR APP_NAME = TEXT("Game");
const LPCTSTR APP_TITLE = TEXT("Game Client");
int PASCAL WinMain(	HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	HCURSOR cursor = LoadCursor( 0, IDC_ARROW );
	HBRUSH brush = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WNDCLASS wc = { CS_HREDRAW | CS_VREDRAW, BSProc, 0, 0, hInstance, NULL,	cursor, brush, NULL, APP_NAME };

	if( !RegisterClass( &wc ) )
		return FALSE;

	return BSMain(	hInstance, lpCmdLine, nCmdShow, APP_NAME, APP_TITLE );
}
