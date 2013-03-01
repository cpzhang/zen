#pragma once
#include "misc/atlHead.h"
#include "resource.h"
#include "TrackBarCtrlWrapper.h"
class TerrainTextureDlg :
	public CDialogImpl<TerrainTextureDlg>
{
public:
	enum { IDD = IDD_DIALOG_TerrainTexture};
	BEGIN_MSG_MAP(TerrainTextureDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()
private:
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	bool upateRadius(const EventArgs&);
	bool upateStrength(const EventArgs&);
	bool upateRadiusInner(const EventArgs&);
	bool upateRadiusOuter(const EventArgs&);
private:
	TrackBarCtrlWrapper BrushStrength_;
	TrackBarCtrlWrapper BrushRadius_;
	TrackBarCtrlWrapper BrushRadiusInner_;
	TrackBarCtrlWrapper BrushRadiusOuter_;
};