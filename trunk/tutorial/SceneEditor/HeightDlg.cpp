#include "HeightDlg.h"
#include "Global.h"
#include "render/Decal.h"
LRESULT RadiusTrackBarCtrl::OnNMThemeChangedSlider( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	int nPercent;
	switch ((int)LOWORD(wParam)) 
	{
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		nPercent = (short)HIWORD(wParam);
		bHandled = TRUE;
		break;
	case SB_ENDSCROLL:
	case SB_LINELEFT:
	case SB_LINERIGHT:
	case SB_PAGELEFT:
	case SB_PAGERIGHT:
		nPercent = GetPos();
		bHandled = TRUE;
		break;
	default:
		break;
	}
	if (bHandled)
	{
		getGlobal()->getBrushDecal()->setRadius(nPercent);
	}
	return bHandled;
}

LRESULT HeightDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled )
{
	bHandled = false;
	radius_.SubclassWindow(GetDlgItem(IDC_SLIDER_BrushSize));
	radius_.SetRange(0, 100);
	radius_.SetPos(50);
	//
	strength_.SubclassWindow(GetDlgItem(IDC_SLIDER_BrushStrength));
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

LRESULT StrengthTrackBarCtrl::OnNMThemeChangedSlider( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	int nPercent;
	switch ((int)LOWORD(wParam)) 
	{
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		nPercent = (short)HIWORD(wParam);
		bHandled = TRUE;
		break;
	case SB_ENDSCROLL:
	case SB_LINELEFT:
	case SB_LINERIGHT:
	case SB_PAGELEFT:
	case SB_PAGERIGHT:
		nPercent = GetPos();
		bHandled = TRUE;
		break;
	default:
		break;
	}
	if (bHandled)
	{
		getGlobal()->setBrushStrength(nPercent * 0.01f);
	}
	return bHandled;
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
