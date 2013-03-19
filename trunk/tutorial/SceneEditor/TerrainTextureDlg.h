#pragma once
#include "common/atlHead.h"
#include "resource.h"
#include "TrackBarCtrlWrapper.h"
#include "IdleHandler.h"
class TerrainTextureDlg :
	public CDialogImpl<TerrainTextureDlg>
		,public IdleHandler
{
public:
	enum { IDD = IDD_DIALOG_TerrainTexture};
	BEGIN_MSG_MAP(TerrainTextureDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		NOTIFY_HANDLER(IDC_LIST_TerrainTexture, NM_DBLCLK, OnNMDblclkListIcons)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	IdleHandler_Derived
private:
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	bool upateStrength(const EventArgs&);
	bool upateRadiusInner(const EventArgs&);
	bool upateRadiusOuter(const EventArgs&);
	void refreshModelIcons();
	LRESULT OnNMDblclkListIcons(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
private:
	TrackBarCtrlWrapper BrushStrength_;
	TrackBarCtrlWrapper BrushRadiusInner_;
	TrackBarCtrlWrapper BrushRadiusOuter_;
	//
	tstring PathSelected_;
	std::vector< std::string > FilePaths_;
	CImageList ImageList_;
	CListViewCtrl Icons_;
};