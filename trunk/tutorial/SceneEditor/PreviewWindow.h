#pragma once
#include "misc/stdHead.h"
#include "misc/atlHead.h"
#include "render/OrbitCamera.h"
class EntityInstance;
class Node;
class PreviewWindow : public CWindowImpl<PreviewWindow, CStatic>
{
public:
	BEGIN_MSG_MAP(PreviewWindow)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_MOUSEMOVE, onMouseMove)
		MESSAGE_HANDLER(WM_MOUSEWHEEL, onMouseWheel)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, onMouseLeftButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, onMouseLeftButtonUp)
		MESSAGE_HANDLER(WM_RBUTTONDOWN, onMouseRightButtonDown)
		MESSAGE_HANDLER(WM_RBUTTONUP, onMouseRightButtonUp)
	END_MSG_MAP()
public:
	PreviewWindow();
	~PreviewWindow();
private:
	LRESULT onMouseWheel(UINT, WPARAM wParam, LPARAM, BOOL& b);
	LRESULT onMouseLeftButtonDown(UINT, WPARAM, LPARAM lParam, BOOL& b);
	LRESULT onMouseLeftButtonUp(UINT, WPARAM, LPARAM lParam, BOOL& b);
	LRESULT onMouseRightButtonDown(UINT, WPARAM, LPARAM lParam, BOOL& b);
	LRESULT onMouseRightButtonUp(UINT, WPARAM, LPARAM lParam, BOOL& b);
	LRESULT onMouseMove(UINT, WPARAM, LPARAM lParam, BOOL& b) ;
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
public:
	void destroy();
	void render();
	void onIdle(float delta);
	void setModel(const std::string resID);
	std::string getModelResID() const;
	std::string saveBackBuffer(const std::string resID);
private:
	EntityInstance* EntityInstance_;
	Node* Hero_;
	OrbitCamera Camera_;
};
