#pragma once
#include "misc/atlHead.h"
#include "misc/stdHead.h"
#include "IdleHandler.h"
#include "render/MouseCamera.h"
#include "render/orbitcamera.h"
#include "render/rendercontext.h"
class Decal;
class Texture;
class ViewWindow: 
	public CWindowImpl<ViewWindow>
	,public IdleHandler
{
public:
	BEGIN_MSG_MAP(ViewWindow)
		MESSAGE_HANDLER(WM_CREATE, onCreate)
//		MESSAGE_HANDLER(WM_PAINT, onPaint)
		MESSAGE_HANDLER(WM_DESTROY, onDestroy)
		MESSAGE_HANDLER(WM_SIZE, onSize)
		MESSAGE_HANDLER(WM_MOUSEWHEEL, onMouseWheel)
		MESSAGE_HANDLER(WM_MOUSEMOVE, onMouseMove)
		MESSAGE_HANDLER(WM_LBUTTONUP, onMouseLeftButtonUp)
		MESSAGE_HANDLER(WM_KEYDOWN, onKeyDown)
	END_MSG_MAP()

	IdleHandler_Derived
public:
	ViewWindow();
	~ViewWindow();
public:
	LRESULT onKeyDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT onCreate(UINT, WPARAM, LPARAM, BOOL&) ;
//	LRESULT onPaint(UINT, WPARAM, LPARAM, BOOL& b) ;
	LRESULT onMouseWheel(UINT, WPARAM wParam, LPARAM, BOOL& b);
	LRESULT onMouseMove(UINT, WPARAM, LPARAM lParam, BOOL& b) ;
	LRESULT onMouseLeftButtonUp(UINT, WPARAM, LPARAM lParam, BOOL& b) ;
	LRESULT onSize(UINT, WPARAM, LPARAM lParam, BOOL& b) ;
	LRESULT onDestroy(UINT, WPARAM, LPARAM, BOOL&) ;
	//
	void run();
public:
	Decal* getBrushDecal();
	//
private:
	float					_fps;
	//OrbitCamera camera_;
	MouseCamera	camera_;
};

