#include "TerrainTextureDlg.h"
#include "StateManager.h"
#include "render/Decal.h"
#include "Global.h"
LRESULT TerrainTextureDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	//
	BrushStrength_.SubclassWindow(GetDlgItem(IDC_SLIDER_TerrainTexture_BrushStrength));
	BrushStrength_.SetRange(1, 255);
	BrushStrength_.SetPos(10);
	BrushStrength_.registerCallback(SubscriberSlot(&TerrainTextureDlg::upateStrength, this));
	//
	//
	BrushRadiusInner_.SubclassWindow(GetDlgItem(IDC_SLIDER_TerrainTexture_RadiusInner));
	BrushRadiusInner_.SetRange(2, 10);
	BrushRadiusInner_.SetPos(3);
	BrushRadiusInner_.registerCallback(SubscriberSlot(&TerrainTextureDlg::upateRadiusInner, this));
	//
	BrushRadiusOuter_.SubclassWindow(GetDlgItem(IDC_SLIDER_TerrainTexture_RadiusOuter));
	BrushRadiusOuter_.SetRange(3, 20);
	BrushRadiusOuter_.SetPos(6);
	BrushRadiusOuter_.registerCallback(SubscriberSlot(&TerrainTextureDlg::upateRadiusOuter, this));
	return 1;
}

bool TerrainTextureDlg::upateStrength( const EventArgs& args)
{
	const TrackBarCtrlWrapperEventArgs* m = (TrackBarCtrlWrapperEventArgs*)&args;
	getStateManager()->getCurrentState()->setBrushStrength(m->nPercent);
	return true;
}

bool TerrainTextureDlg::upateRadiusInner( const EventArgs& args)
{
	const TrackBarCtrlWrapperEventArgs* m = (TrackBarCtrlWrapperEventArgs*)&args;
	getStateManager()->getCurrentState()->setBrushRadiusInner(m->nPercent);
	return true;
}

bool TerrainTextureDlg::upateRadiusOuter( const EventArgs& args)
{
	const TrackBarCtrlWrapperEventArgs* m = (TrackBarCtrlWrapperEventArgs*)&args;
	getStateManager()->getCurrentState()->setBrushRadiusOuter(m->nPercent);
	return true;
}
