#include "HeightDlg.h"
#include "Global.h"
#include "render/Decal.h"
#include "StateManager.h"

LRESULT HeightDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled )
{
	bHandled = false;
	radius_.SubclassWindow(GetDlgItem(IDC_SLIDER_BrushSize));
	radius_.SetRange(0, 100);
	radius_.SetPos(50);
	radius_.registerCallback(SubscriberSlot(&HeightDlg::upateRadius, this));
	//
	strength_.SubclassWindow(GetDlgItem(IDC_SLIDER_BrushStrength));
	strength_.registerCallback(SubscriberSlot(&HeightDlg::upateStrength, this));
	strength_.SetRange(0, 100);
	strength_.SetPos(50);
	//
	aHeight_ = 0;
	// First DDX call, hooks up variables to controls.
	DoDataExchange(false); 
	//
	CenterWindow();
	return TRUE;
}

LRESULT HeightDlg::OnBnClickedCheckAbsoluteheight( WORD wNotifyCode, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	getGlobal()->setIsAbsoluteHeight(!getGlobal()->isAbosoluteHeight());
	return 0;
}

LRESULT HeightDlg::OnEnChangeEditAbsoluteheight(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled)
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д __super::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	bHandled = TRUE;
	DoDataExchange(true);
	if (getGlobal())
	{
		getGlobal()->setAbsoluteHeight(aHeight_);
	}
	
	return bHandled;
}

LRESULT HeightDlg::OnBnClickedCheckSmoothAverage( WORD wNotifyCode, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	getGlobal()->setIsSmoothAverage(!getGlobal()->isSmoothAverage());
	return 0;
}

void HeightDlg::onIdle(const float delta)
{
}

void HeightDlg::onRefreshLuaScript()
{
}

bool HeightDlg::upateRadius( const EventArgs& args)
{
	const TrackBarCtrlWrapperEventArgs* m = (TrackBarCtrlWrapperEventArgs*)&args;
	getGlobal()->getBrushDecal()->setRadius(m->nPercent);
	return true;
}

bool HeightDlg::upateStrength( const EventArgs& args)
{
	const TrackBarCtrlWrapperEventArgs* m = (TrackBarCtrlWrapperEventArgs*)&args;
	getStateManager()->getCurrentState()->setBrushStrength(m->nPercent);
	return true;
}
