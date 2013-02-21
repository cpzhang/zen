#pragma once
#include "misc/stdHead.h"
#include "misc/atlHead.h"
class EntityInstance;
class Node;
class PreviewWindow : public CWindowImpl<PreviewWindow, CStatic>
{
public:
	BEGIN_MSG_MAP(PreviewWindow)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_MOUSEMOVE, onMouseMove)
	END_MSG_MAP()
public:
	PreviewWindow();
	~PreviewWindow();
private:
	LRESULT onMouseMove(UINT, WPARAM, LPARAM lParam, BOOL& b) ;
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
public:
	void render();
	void setModel(EntityInstance* m);
private:
	Node* Hero_;
};
