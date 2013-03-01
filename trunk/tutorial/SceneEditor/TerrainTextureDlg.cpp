#include "TerrainTextureDlg.h"
#include "StateManager.h"
#include "render/Decal.h"
#include "Global.h"
LRESULT TerrainTextureDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	BrushRadius_.SubclassWindow(GetDlgItem(IDC_SLIDER_TerrainTexture_BrushRadius));
	BrushRadius_.SetRange(0, 100);
	BrushRadius_.SetPos(50);
	BrushRadius_.registerCallback(SubscriberSlot(&TerrainTextureDlg::upateRadius, this));
	//
	BrushStrength_.SubclassWindow(GetDlgItem(IDC_SLIDER_TerrainTexture_BrushStrength));
	BrushStrength_.SetRange(0, 100);
	BrushStrength_.SetPos(50);
	BrushStrength_.registerCallback(SubscriberSlot(&TerrainTextureDlg::upateStrength, this));
	return 1;
}
bool TerrainTextureDlg::upateRadius( const EventArgs& args)
{
	const TrackBarCtrlWrapperEventArgs* m = (TrackBarCtrlWrapperEventArgs*)&args;
	getGlobal()->getBrushDecal()->setRadius(m->nPercent);
	return true;
}

bool TerrainTextureDlg::upateStrength( const EventArgs& args)
{
	const TrackBarCtrlWrapperEventArgs* m = (TrackBarCtrlWrapperEventArgs*)&args;
	getStateManager()->getCurrentState()->setBrushStrength(m->nPercent);
	return true;
}